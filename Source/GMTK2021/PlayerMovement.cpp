// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMovement.h"

#include "PlayerCharacter.h"

static TAutoConsoleVariable<int32> CVarShowPos(TEXT("cl.ShowPos"), 0, TEXT("Show position and movement information.\n"), ECVF_Default);

UPlayerMovement::UPlayerMovement()
{
	// We have our own air movement handling, so we can allow for full air
	// control through UE's logic
	AirControl = 1.0f;

	// Disable air control boost
	AirControlBoostMultiplier = 1.0f;
	AirControlBoostVelocityThreshold = 0.0f;

	// HL2 cl_(forward & side)speed = 450Hu
	MaxAcceleration = 857.25f;

	// Set the default walk speed
	MaxWalkSpeed = 361.9f;
	WalkSpeed = 285.75f;
	RunSpeed = 361.9f;
	SprintSpeed = 609.6f;

	GroundAccelerationMultiplier = 10.0f;
	AirAccelerationMultiplier = 3.0f;

	// 30 air speed cap from HL2
	AirSpeedCap = 57.15f;

	// HL2 like friction
	// sv_friction
	GroundFriction = 4.0f;
	BrakingFriction = 4.0f;
	bUseSeparateBrakingFriction = false;

	// No multiplier
	BrakingFrictionFactor = 1.0f;

	// Braking deceleration (sv_stopspeed)
	FallingLateralFriction = 0.0f;
	BrakingDecelerationFalling = 0.0f;
	BrakingDecelerationFlying = 190.5f;
	BrakingDecelerationSwimming = 190.5f;
	BrakingDecelerationWalking = 190.5f;

	// HL2 step height
	MaxStepHeight = 34.29f;
	// Step height scaling due to speed
	MinStepHeight = 7.5f;
	
	// Jump strength
	JumpZVelocity = 354.8f;
	// Always have the same jump
	JumpOffJumpZFactor = 1.0f;

	// Speed multiplier bounds
	SpeedMultMin = SprintSpeed * 1.7f;
	SpeedMultMax = SprintSpeed * 2.5f;

	// Start out braking
	bBrakingFrameTolerated = true;

	// Crouching
	CrouchedHalfHeight = 34.29f;
	MaxWalkSpeedCrouched = 120.65f;
	bCanWalkOffLedgesWhenCrouching = true;

	// Slope angle is 45.57 degrees
	SetWalkableFloorZ(0.7f);

	// Tune physics interactions
	StandingDownwardForceScale = 1.0f;

	// Reasonable values polled from NASA (https://msis.jsc.nasa.gov/sections/section04.htm#Figure%204.9.3-6)
	// and Standard Handbook of Machine Design
	InitialPushForceFactor = 100.0f;
	PushForceFactor = 500.0f;

	RepulsionForce = 0.0f;
	MaxTouchForce = 0.0f;
	TouchForceFactor = 0.0f;

	// Just push all objects based on their impact point
	// it might be weird with a lot of dev objects due to scale, but
	// it's much more realistic.
	bPushForceUsingZOffset = false;
	PushForcePointZOffsetFactor = -0.66f;

	// Scale push force down if we are slow
	bScalePushForceToVelocity = true;

	// Don't push more if there's more mass
	bPushForceScaledToMass = false;
	bTouchForceScaledToMass = false;
	Mass = 90.71f;

	// Don't smooth rotation at all
	bUseControllerDesiredRotation = false;

	// Flat base
	bUseFlatBaseForFloorChecks = true;

	// Agent props
	NavAgentProps.bCanCrouch = true;
	NavAgentProps.bCanFly = true;

	// Default showpos to false
	bShowPos = true;

	Character = Cast<APlayerCharacter>(GetOwner());
}

void UPlayerMovement::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	bAppliedFriction = false;

	// TODO(mastercoms): HACK: double friction in order to account for insufficient braking on substepping
	if (DeltaTime > MaxSimulationTimeStep)
	{
		BrakingFrictionFactor = 2.0f;
	}
	else
	{
		BrakingFrictionFactor = 1.0f;
	}

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if ((bShowPos || CVarShowPos->GetInt() != 0) && CharacterOwner)
	{
		GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Green,
			FString::Printf(TEXT("pos: %f %f %f"), CharacterOwner->GetActorLocation().X, CharacterOwner->GetActorLocation().Y,
				CharacterOwner->GetActorLocation().Z));
		GEngine->AddOnScreenDebugMessage(2, 1.0f, FColor::Green,
			FString::Printf(TEXT("ang: %f %f %f"), CharacterOwner->GetControlRotation().Yaw,
				CharacterOwner->GetControlRotation().Pitch, CharacterOwner->GetControlRotation().Roll));
		GEngine->AddOnScreenDebugMessage(3, 1.0f, FColor::Green, FString::Printf(TEXT("vel: %f"), FMath::Sqrt(Velocity.X * Velocity.X + Velocity.Y * Velocity.Y)));
	}
}

void UPlayerMovement::CalcVelocity(float DeltaTime, float Friction, bool bFluid, float BrakingDeceleration)
{
	if (CharacterOwner && CharacterOwner->GetLocalRole() == ROLE_SimulatedProxy)
	{
		return;
	}

	FVector input = GetLastInputVector();

	float MaxSpeed = GetMaxSpeed();

	const bool bIsGroundMove = IsMovingOnGround() && bBrakingFrameTolerated;

	float SurfaceFriction = 1.0f;


	if (bIsGroundMove && !bAppliedFriction)
	{
		const float ActualBrakingFriction = Friction * SurfaceFriction;
		ApplyVelocityBraking(DeltaTime, ActualBrakingFriction, BrakingDeceleration);
		bAppliedFriction = true;
	}


	// Clamp acceleration to max speed
	Acceleration = Acceleration.GetClampedToMaxSize2D(MaxSpeed);
	// Find veer
	const FVector AccelDir = Acceleration.GetSafeNormal2D();
	const float Veer = Velocity.X * AccelDir.X + Velocity.Y * AccelDir.Y;
	// Get add speed with air speed cap
	const float AddSpeed = (bIsGroundMove ? Acceleration : Acceleration.GetClampedToMaxSize2D(AirSpeedCap)).Size2D() - Veer;
	if (AddSpeed > 0.0f)
	{
		// Apply acceleration
		float AccelerationMultiplier = bIsGroundMove ? GroundAccelerationMultiplier : AirAccelerationMultiplier;
		Acceleration *= AccelerationMultiplier * SurfaceFriction * DeltaTime;
		Acceleration = Acceleration.GetClampedToMaxSize2D(AddSpeed);
		Velocity += Acceleration;
	}

	Velocity = Velocity.GetClampedToMaxSize2D(13470.4f);
}

void UPlayerMovement::ApplyVelocityBraking(float DeltaTime, float Friction, float BrakingDeceleration)
{
	float Speed = Velocity.Size2D();
	if (Speed <= 0.1f || !HasValidData() || HasAnimRootMotion() || DeltaTime < MIN_TICK_TIME)
	{
		return;
	}

	const float FrictionFactor = FMath::Max(0.0f, BrakingFrictionFactor);
	Friction = FMath::Max(0.0f, Friction * FrictionFactor);
	BrakingDeceleration = FMath::Max(BrakingDeceleration, Speed);
	BrakingDeceleration = FMath::Max(0.0f, BrakingDeceleration);
	const bool bZeroFriction = FMath::IsNearlyZero(Friction);
	const bool bZeroBraking = BrakingDeceleration == 0.0f;

	if (bZeroFriction || bZeroBraking)
	{
		return;
	}

	const FVector OldVel = Velocity;

	// Decelerate to brake to a stop
	const FVector RevAccel = Friction * BrakingDeceleration * Velocity.GetSafeNormal();
	Velocity -= RevAccel * DeltaTime;

	// Don't reverse direction
	if ((Velocity | OldVel) <= 0.0f)
	{
		Velocity = FVector::ZeroVector;
		return;
	}

	// Clamp to zero if nearly zero, or if below min threshold and braking.
	const float VSizeSq = Velocity.SizeSquared();
	if (VSizeSq <= KINDA_SMALL_NUMBER)
	{
		Velocity = FVector::ZeroVector;
	}
}

float UPlayerMovement::GetMaxSpeed() const
{
	if (IsWalking() || bCheatFlying || IsFalling())
	{
		if (Character->IsSprinting())
		{
			if (IsCrouching())
			{
				return MaxWalkSpeedCrouched * 1.7f;
			}
			return bCheatFlying ? SprintSpeed * 1.5f : SprintSpeed;
		}
		if (Character->DoesWantToWalk())
		{
			return WalkSpeed;
		}
		if (IsCrouching())
		{
			return MaxWalkSpeedCrouched;
		}
	}

	return Super::GetMaxSpeed();
}

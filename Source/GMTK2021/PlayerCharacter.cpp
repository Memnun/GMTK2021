// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "PlayerMovement.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

// Sets default values
APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<UPlayerMovement>(ACharacter::CharacterMovementComponentName))
{

    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    GetCapsuleComponent()->InitCapsuleSize(30.48f, 68.58f);

    BaseTurnRate = 45.0f;
    BaseLookUpRate = 45.0f;

    BaseEyeHeight = 53.34f;

    MovementPtr = Cast<UPlayerMovement>(ACharacter::GetMovementComponent());

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComponent->SetupAttachment(RootComponent);
    CameraComponent->SetRelativeLocation(FVector(0, 0, BaseEyeHeight));
    CameraComponent->bUsePawnControlRotation = true;
	CameraComponent->bConstrainAspectRatio = true;
	CameraComponent->SetNetAddressable();
	CameraComponent->SetIsReplicated(true);

	/*
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Weapon Spring Arm"));
    SpringArmComponent->bEnableCameraRotationLag = true;
    SpringArmComponent->CameraRotationLagSpeed = 55.0f;
    SpringArmComponent->TargetArmLength = 0.0f;
    SpringArmComponent->bDoCollisionTest = true;
    SpringArmComponent->AttachTo(CameraComponent);
	SpringArmComponent->SetIsReplicated(true);
	*/

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    MaxJumpTime = -4.0f * GetCharacterMovement()->JumpZVelocity / (3.0f * GetCharacterMovement()->GetGravityZ());

}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Weapon swaying
	/*
	if (CurrentWeapon) {

		float SwayXFactor = -GetInputAxisValue("turn") * 2.0f;
		float SwayYFactor = GetInputAxisValue("lookup") * 2.0f + -(CameraComponent->GetComponentRotation().Pitch / 67.5f);
		UE_LOG(LogTemp, Warning, TEXT("%f"), CameraComponent->GetComponentRotation().Pitch);

		float MaxSway = 5.f;
		float SwaySmooth = 5.0f;

		float LeadXFactor = GetInputAxisValue("turn") * 1.0f;
		float LeadYFactor = -GetInputAxisValue("lookup") * 1.0f;

		SwayXFactor = FMath::Clamp(SwayXFactor, -MaxSway, MaxSway);
		SwayYFactor = FMath::Clamp(SwayYFactor, -MaxSway, MaxSway);
		LeadXFactor = FMath::Clamp(LeadXFactor, -MaxSway, MaxSway);
		LeadYFactor = FMath::Clamp(LeadYFactor, -MaxSway, MaxSway);

		const FVector TargetLocation = FVector(0, SwayXFactor, SwayYFactor) + WeaponOffset;
		const FVector RelativeLocation = CameraComponent->GetComponentTransform().InverseTransformPosition(CurrentWeapon->GetActorLocation());
		const FVector ResultLocation = FMath::VInterpTo(RelativeLocation, TargetLocation, DeltaTime, SwaySmooth);
		CurrentWeapon->SetActorRelativeLocation(ResultLocation);

		const FRotator TargetRotation = FRotator(LeadYFactor, LeadXFactor, 0);
		const FRotator RelativeRotation = CameraComponent->GetComponentTransform().InverseTransformRotation(CurrentWeapon->GetActorRotation().Quaternion()).Rotator();
		const FRotator ResultRotation = FMath::RInterpTo(RelativeRotation, TargetRotation, DeltaTime, SwaySmooth);
		CurrentWeapon->SetActorRelativeRotation(ResultRotation);

	}
*/
}

void APlayerCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::Turn(float Rate)
{
	Rate = Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds();

	AddControllerYawInput(Rate);
}

void APlayerCharacter::LookUp(float Rate)
{
	Rate = Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds();

	AddControllerPitchInput(Rate);
}

void APlayerCharacter::ClearJumpInput(float DeltaTime)
{
	Super::ClearJumpInput(DeltaTime);
}

void APlayerCharacter::OnJumped_Implementation()
{
	LastJumpTime = GetWorld()->GetTimeSeconds();
	if (GetWorld()->GetTimeSeconds() >= LastJumpBoostTime + MaxJumpTime)
	{
		LastJumpBoostTime = GetWorld()->GetTimeSeconds();
		// Boost forward speed on jump
		FVector Facing = GetActorForwardVector();
		// Give it its backward/forward direction
		float ForwardSpeed;
		FVector Input = MovementPtr->GetLastInputVector().GetClampedToMaxSize2D(1.0f) * MovementPtr->GetMaxAcceleration();
		ForwardSpeed = Input | Facing;
		// Adjust how much the boost is
		float SpeedBoostPerc = (bIsSprinting || bIsCrouched) ? 0.1f : 0.5f;
		// How much we are boosting by
		float SpeedAddition = FMath::Abs(ForwardSpeed * SpeedBoostPerc);
		// We can only boost up to this much
		float MaxBoostedSpeed = GetCharacterMovement()->GetMaxSpeed() + GetCharacterMovement()->GetMaxSpeed() * SpeedBoostPerc;
		// Calculate new speed
		float NewSpeed = SpeedAddition + GetMovementComponent()->Velocity.Size2D();
		float SpeedAdditionNoClamp = SpeedAddition;

		// Scale the boost down if we are going over
		if (NewSpeed > MaxBoostedSpeed)
		{
			SpeedAddition -= NewSpeed - MaxBoostedSpeed;
		}

		if (ForwardSpeed < -MovementPtr->GetMaxAcceleration() * FMath::Sin(0.6981f))
		{
			// Boost backwards if we're going backwards
			SpeedAddition *= -1.0f;
			SpeedAdditionNoClamp *= -1.0f;
		}

		// Boost our velocity
		FVector JumpBoostedVel = GetMovementComponent()->Velocity;
//		FVector JumpBoostedVel = GetMovementComponent()->Velocity + Facing * SpeedAddition;
        float JumpBoostedSizeSq = JumpBoostedVel.SizeSquared2D();

        FVector JumpBoostedUnclampVel = GetMovementComponent()->Velocity + Facing * SpeedAdditionNoClamp;
        float JumpBoostedUnclampSizeSq = JumpBoostedUnclampVel.SizeSquared2D();
        if (JumpBoostedUnclampSizeSq > JumpBoostedSizeSq)
        {
            JumpBoostedVel = JumpBoostedUnclampVel;
            JumpBoostedSizeSq = JumpBoostedUnclampSizeSq;
        }
        if (GetMovementComponent()->Velocity.SizeSquared2D() < JumpBoostedSizeSq)
        {
            GetMovementComponent()->Velocity = JumpBoostedVel;
        }
    }
}

bool APlayerCharacter::CanJumpInternal_Implementation() const
{
    bool bCanJump = GetCharacterMovement() && GetCharacterMovement()->IsJumpAllowed();

    if (bCanJump)
    {
        // Ensure JumpHoldTime and JumpCount are valid.
        if (!bWasJumping || GetJumpMaxHoldTime() <= 0.0f)
        {
            if (JumpCurrentCount == 0 && GetCharacterMovement()->IsFalling())
            {
                bCanJump = JumpCurrentCount + 1 < JumpMaxCount;
            }
            else
            {
                bCanJump = JumpCurrentCount < JumpMaxCount;
            }
        }
        else
        {
            // Only consider JumpKeyHoldTime as long as:
            // A) We are on the ground
            // B) The jump limit hasn't been met OR
            // C) The jump limit has been met AND we were already jumping
            const bool bJumpKeyHeld = (bPressedJump && JumpKeyHoldTime < GetJumpMaxHoldTime());
            bCanJump = bJumpKeyHeld &&
                (GetCharacterMovement()->IsMovingOnGround() || (JumpCurrentCount < JumpMaxCount) || (bWasJumping && JumpCurrentCount == JumpMaxCount));
        }
        if (GetCharacterMovement()->IsMovingOnGround())
        {
            float FloorZ = FVector(0.0f, 0.0f, 1.0f) | GetCharacterMovement()->CurrentFloor.HitResult.ImpactNormal;
            float WalkableFloor = GetCharacterMovement()->GetWalkableFloorZ();
            bCanJump &= (FloorZ >= WalkableFloor || FMath::IsNearlyEqual(FloorZ, WalkableFloor));
        }
    }

    return bCanJump;
}

void APlayerCharacter::Sprint()
{
	bIsSprinting = true;
}

void APlayerCharacter::StopSprinting()
{
	bIsSprinting = false;
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAction("jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("jump", IE_Released, this, &ACharacter::StopJumping);

    PlayerInputComponent->BindAction("sprint", IE_Pressed, this, &APlayerCharacter::Sprint);
    PlayerInputComponent->BindAction("sprint", IE_Released, this, &APlayerCharacter::StopSprinting);

    PlayerInputComponent->BindAxis("forward", this, &APlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis("right", this, &APlayerCharacter::MoveRight);

    PlayerInputComponent->BindAxis("turn", this, &APlayerCharacter::Turn);
    PlayerInputComponent->BindAxis("lookup", this, &APlayerCharacter::LookUp);
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "PlayerMovement.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"
#include "Weapon.h"
#include "HealthComponent.h"

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

    MaxJumps = 2;
    JumpsRemaining = MaxJumps;
    WallJumpVelocity = 200.f;

    MouseSensitivity = 1.f;

    RollAngle = 2.0f;
    RollSpeed = 200.f;

    BobSpeed = 0.6f;
    BobAmount = 0.2f;

    MovementPtr = Cast<UPlayerMovement>(ACharacter::GetMovementComponent());

    HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComponent->SetupAttachment(RootComponent);
    CameraComponent->SetRelativeLocation(FVector(0, 0, BaseEyeHeight));
    CameraComponent->bUsePawnControlRotation = true;
    CameraComponent->bConstrainAspectRatio = true;
    CameraComponent->SetNetAddressable();
    CameraComponent->SetIsReplicated(true);

    NormalFieldOfView = CameraComponent->FieldOfView;
    WallRunFieldOfView = 110.f;
    TargetFieldOfView = NormalFieldOfView;

    ViewSwayAmount = 1.0f;
    ViewWeaponLeadAmount = 2.0f;
    MaxWeaponSway = 2.5f;

    WeaponOffset = FVector(0);

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

    GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &APlayerCharacter::OnComponentHit);


    // Spawn in weapon view model
    SetCurrentWeaponByClass(SpawnWeapon);

}

void APlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);


    FRotator CameraRotation = GetController()->GetControlRotation();
    FRotator CameraTargetRotation = FRotator(CameraRotation.Pitch, CameraRotation.Yaw, CameraTargetRoll);
    GetController()->SetControlRotation(FMath::RInterpTo(CameraRotation, CameraTargetRotation, DeltaTime, 10.f));

    //GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, FString::Printf(TEXT("%f"), CalculateViewRoll()));

    CameraComponent->FieldOfView = FMath::FInterpTo(CameraComponent->FieldOfView, TargetFieldOfView, DeltaTime, 5.f);

    if (!bIsWallRunning && MovementPtr->IsWalking())
        GetController()->SetControlRotation(FRotator(CameraRotation.Pitch, CameraRotation.Yaw, CalculateViewRoll()));

    // Weapon swaying
    CalculateWeaponSway(DeltaTime);
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

        ForwardAxis = Value;
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

    RightAxis = Value;
}

void APlayerCharacter::Turn(float Rate)
{
    Rate = Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds() * MouseSensitivity;

    AddControllerYawInput(Rate);
}

void APlayerCharacter::LookUp(float Rate)
{
    Rate = Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds() * MouseSensitivity;

    AddControllerPitchInput(Rate);
}

void APlayerCharacter::OnJumped_Implementation()
{
    GetMovementComponent()->Velocity = FindLaunchVelocity();

    if (bIsWallRunning)
        EndWallRun(EWallRunEndReason::JUMP);


    JumpBoost();
    
}

bool APlayerCharacter::CanJumpInternal_Implementation() const
{
    //bool bCanJump = GetCharacterMovement() && GetCharacterMovement()->IsJumpAllowed();

    // This is a bit of a hack to make double jump work
    bool bCanJump = true;

    /*
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
*/

    return bCanJump;
}

void APlayerCharacter::Sprint()
{
    if(bCanSpeedBoost)
        bIsSprinting = true;
}

void APlayerCharacter::StopSprinting()
{
    bIsSprinting = false;
}

void APlayerCharacter::DoJump()
{
    if (ConsumeJump())
    {
        Jump();

        if (bIsWallRunning)
        {
        }
    }
}

void APlayerCharacter::ResetJump(int Jumps)
{
    JumpsRemaining = FMath::Clamp<int>(Jumps, 0, MaxJumps);
}

void APlayerCharacter::BeginWallRun()
{
    if(!bCanWallRun) return;
    bIsWallRunning = true;
    BeginCameraTilt();
    GetWorld()->GetTimerManager().SetTimer(WallRunHandle, this, &APlayerCharacter::UpdateWallRun, 0.05f, true);
    TargetFieldOfView = WallRunFieldOfView;

    if (MovementPtr->Velocity.Z < 0)
    {
        MovementPtr->GravityScale = 0.f;
        MovementPtr->Velocity.Z = 0;
    }
    
}

void APlayerCharacter::UpdateWallRun()
{
    if (MovementPtr->Velocity.Z < 0)
    {
        MovementPtr->GravityScale = 0.f;
        MovementPtr->Velocity.Z = 0;
    }

    // Slight vertical climb
    MovementPtr->Velocity.Z = WallRunClimbVelocity;


    FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("Trace")), false, this);

    //Re-initialize hit info
    FHitResult Hit(ForceInit);

    FVector WallDirection = FVector::CrossProduct(WallRunDirection, WallRunSide == EWallRunSide::RIGHT ? FVector(0.f, 0.f, 1.f) : FVector(0.f, 0.f, -1.f));
    FVector Start = GetActorLocation();
    FVector End = Start + WallDirection * 200.f;
    //DrawDebugDirectionalArrow(GetWorld(), Start, End, 100.f, FColor::Red, true, 3.0f);

    //call GetWorld() from within an actor extending class
    GetWorld()->LineTraceSingleByChannel(
        Hit,        //result
        Start,    //start
        End, //end
        ECC_Pawn, //collision channel
        TraceParams
    );


    PrevWallRunSide = WallRunSide;

    FindRunDirectionAndSide(Hit.ImpactNormal);

    if (!CheckRequiredKeys() || !Hit.bBlockingHit || PrevWallRunSide != WallRunSide)
    {
        EndWallRun(EWallRunEndReason::FALL);
        return;
    }

    MovementPtr->Velocity.X = WallRunDirection.X * MovementPtr->GetMaxSpeed() * 1.5f;
    MovementPtr->Velocity.Y = WallRunDirection.Y * MovementPtr->GetMaxSpeed() * 1.5f;
}

void APlayerCharacter::EndWallRun(EWallRunEndReason Reason)
{
    switch (Reason)
    {
    case EWallRunEndReason::FALL:
        ResetJump(1);
        break;
    case EWallRunEndReason::JUMP:
        ResetJump(MaxJumps - 1);
        break;
    }

    MovementPtr->GravityScale = 1.f;

    EndCameraTilt();
    TargetFieldOfView = NormalFieldOfView;

    GetWorld()->GetTimerManager().ClearTimer(WallRunHandle);

    bIsWallRunning = false;

}

void APlayerCharacter::BeginCameraTilt()
{
    CameraTargetRoll = 15.0f * (WallRunSide == EWallRunSide::LEFT ? -1.f : 1.f);

}

void APlayerCharacter::EndCameraTilt()
{
    CameraTargetRoll = 0.0f;
}

void APlayerCharacter::FindRunDirectionAndSide(FVector WallNormal)
{
    FVector2D WallNormal2D = FVector2D(WallNormal.X, WallNormal.Y);
    FVector2D RightVector2D = FVector2D(GetActorRightVector().X, GetActorRightVector().Y);

    WallRunSide = FVector2D::DotProduct(WallNormal2D, RightVector2D) > 0 ? EWallRunSide::RIGHT : EWallRunSide::LEFT;
    FVector SideAlignment = WallRunSide == EWallRunSide::RIGHT ? FVector(0.f, 0.f, 1.f) : FVector(0.f, 0.f, -1.f);
    WallRunDirection = FVector::CrossProduct(WallNormal, SideAlignment);
}

bool APlayerCharacter::CanSurfaceBeWallRan(FVector SurfaceNormal) const
{
    float WallFaceAngle;
    FVector SurfaceXYNormalized;
    float SurfaceAngle;

    // Check if surface is a ceiling
    if (SurfaceNormal.Z < -0.05f)
    {
        return false;
    }

    WallFaceAngle = FMath::Abs(FVector::DotProduct(SurfaceNormal, GetActorForwardVector()));
    if (WallFaceAngle > .75f)
    {
        return false;
    }

    SurfaceXYNormalized = SurfaceNormal.GetSafeNormal2D();
    SurfaceAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(SurfaceXYNormalized, SurfaceNormal)));

    if (SurfaceAngle < MovementPtr->GetWalkableFloorAngle())
    {
        return true;
    }

    return false;
}

FVector APlayerCharacter::FindLaunchVelocity()
{
    FVector LaunchDirection = GetMovementComponent()->Velocity;

    if (bIsWallRunning)
    {
        FVector WallAlignment = FVector();
        WallAlignment.Z = (WallRunSide == EWallRunSide::LEFT ? 1.f : -1.f) * WallJumpVelocity;
        LaunchDirection += FVector::CrossProduct(WallRunDirection, WallAlignment);
    }

    LaunchDirection.Z = MovementPtr->JumpZVelocity;

    return LaunchDirection;
}

bool APlayerCharacter::CheckRequiredKeys()
{

    if (ForwardAxis > 0.1f)
    {
        // Stops wall running if you're not holding into the wall, but it feels bad
        /*
        switch (WallRunSide)
        {
        case EWallRunSide::LEFT:
            return RightAxis > 0.1f;
            break;
        case EWallRunSide::RIGHT:
            return RightAxis < -0.1f;
            break;
        }
        */
        return true;
    }

    return false;
}

bool APlayerCharacter::ConsumeJump()
{
    bool JumpConsumed = false;


    if (bIsWallRunning)
    {
        JumpConsumed = true;
    }
    else if (JumpsRemaining > 0)
    {
        JumpConsumed = true;
        JumpsRemaining--;
    }

    return JumpConsumed;
}

void APlayerCharacter::JumpBoost()
{

    LastJumpTime = GetWorld()->GetTimeSeconds();

    if (GetWorld()->GetTimeSeconds() >= LastJumpBoostTime + MaxJumpTime && !bIsWallRunning)
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

float APlayerCharacter::CalculateViewRoll()
{
    float	Sign;
    float	Side;
    float	Value;

    if (MovementPtr->Velocity.SizeSquared() < KINDA_SMALL_NUMBER)
        return 0;

    Side = FVector::DotProduct(MovementPtr->Velocity, GetActorRightVector());
    Sign = Side < 0 ? -1 : 1;
    Side = FMath::Abs(Side);


    Value = RollAngle;

	if (Side < RollSpeed)
		Side = Side * Value / RollSpeed;
	else
		Side = Value;
	
	return Side*Sign;
}

void APlayerCharacter::CalculateWeaponSway(float DeltaTime)
{
    if (CurrentWeapon)
    {
        // Weapon bobbing as you run
        float Time = FPlatformTime::ToSeconds(FPlatformTime::Cycles());
        float XBob = 0;
        float YBob = 0;
        float Cycle;
        float VelocityLength;

        float SwayXFactor;
        float SwayYFactor;
        float SwayZFactor;

        VelocityLength = MovementPtr->Velocity.Size2D() / MovementPtr->GetMaxSpeed();

        if (VelocityLength > KINDA_SMALL_NUMBER && MovementPtr->IsWalking())
        {
            Cycle = Time * (2 * PI) * BobSpeed;

            //GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Blue, FString::Printf(TEXT("%f"), Cycle));

            XBob = FMath::Sin(Cycle * BobSpeed) * BobAmount * VelocityLength;
            YBob = FMath::Cos(Cycle * BobSpeed / 2) * (BobAmount * 2.f) * VelocityLength;
        }

        // Weapon swaying as you turn
        SwayYFactor = -GetInputAxisValue("turn") * ViewSwayAmount;
        SwayZFactor = (GetInputAxisValue("lookup") + -(CameraComponent->GetComponentRotation().Pitch / 67.5f) - (MovementPtr->Velocity.Z / 300.f)) * ViewSwayAmount;

        SwayXFactor = XBob;
        SwayYFactor -= YBob;

        float MaxSway = MaxWeaponSway;
        float SwaySmooth = 5.0f;

        float LeadXFactor = GetInputAxisValue("turn") * ViewWeaponLeadAmount;
        float LeadYFactor = -GetInputAxisValue("lookup") * ViewWeaponLeadAmount;

        SwayXFactor = FMath::Clamp(SwayXFactor, -MaxSway, MaxSway);
        SwayYFactor = FMath::Clamp(SwayYFactor, -MaxSway, MaxSway);
        SwayZFactor = FMath::Clamp(SwayZFactor, -MaxSway, MaxSway);
        LeadXFactor = FMath::Clamp(LeadXFactor, -MaxSway, MaxSway);
        LeadYFactor = FMath::Clamp(LeadYFactor, -MaxSway, MaxSway);

        const FVector TargetLocation = FVector(SwayXFactor, SwayYFactor, SwayZFactor) + WeaponOffset;
        const FVector RelativeLocation = CameraComponent->GetComponentTransform().InverseTransformPosition(CurrentWeapon->GetActorLocation());
        const FVector ResultLocation = FMath::VInterpTo(RelativeLocation, TargetLocation, DeltaTime, SwaySmooth);
        CurrentWeapon->SetActorRelativeLocation(ResultLocation);

        const FRotator TargetRotation = FRotator(LeadYFactor, LeadXFactor, 0);
        const FRotator RelativeRotation = CameraComponent->GetComponentTransform().InverseTransformRotation(CurrentWeapon->GetActorRotation().Quaternion()).Rotator();
        const FRotator ResultRotation = FMath::RInterpTo(RelativeRotation, TargetRotation, DeltaTime, SwaySmooth);
        CurrentWeapon->SetActorRelativeRotation(ResultRotation);
    }
}

void APlayerCharacter::FireWeapon()
{
    if (CurrentWeapon)
    {
        CurrentWeapon->TryFire();
    }
}

void APlayerCharacter::StopFireWeapon()
{
    if (CurrentWeapon)
    {
        CurrentWeapon->StopFire();
    }
}

AWeapon* APlayerCharacter::SetCurrentWeaponByClass_Implementation(TSubclassOf<AWeapon> NewWeaponClass)
{
    if(!NewWeaponClass->IsValidLowLevel()) return nullptr;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	AWeapon* Spawner = GetWorld()->SpawnActor<AWeapon>(NewWeaponClass, SpawnParams);

	if (Spawner)
	{
        WeaponOffset = Spawner->WeaponOffset;

        AWeapon* OldWeapon = CurrentWeapon;
		Spawner->AttachToComponent(CameraComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		CurrentWeapon = Spawner;
		CurrentWeapon->SetActorRelativeLocation(WeaponOffset);
        //Broadcast that the player's current weapon has changed, if any objects are listening for that
        if(OnWeaponChanged.IsBound())
            OnWeaponChanged.Broadcast(CurrentWeapon);

        if(OldWeapon)
		{
			OldWeapon->SetActorHiddenInGame(true);
			OldWeapon->SetLifeSpan(.001f);
		}
        return CurrentWeapon;
	}
    return CurrentWeapon;
}

void APlayerCharacter::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!bIsWallRunning && CanSurfaceBeWallRan(Hit.ImpactNormal) && MovementPtr->IsFalling())
    {
        FindRunDirectionAndSide(Hit.ImpactNormal);

        if (CheckRequiredKeys())
        {
            BeginWallRun();
        }
    }
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAction("jump", IE_Pressed, this, &APlayerCharacter::DoJump);
    PlayerInputComponent->BindAction("jump", IE_Released, this, &ACharacter::StopJumping);

    PlayerInputComponent->BindAction("sprint", IE_Pressed, this, &APlayerCharacter::Sprint);
    PlayerInputComponent->BindAction("sprint", IE_Released, this, &APlayerCharacter::StopSprinting);

    PlayerInputComponent->BindAction("shoot", IE_Pressed, this, &APlayerCharacter::FireWeapon);
    PlayerInputComponent->BindAction("shoot", IE_Released, this, &APlayerCharacter::StopFireWeapon);

    PlayerInputComponent->BindAxis("forward", this, &APlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis("right", this, &APlayerCharacter::MoveRight);

    PlayerInputComponent->BindAxis("turn", this, &APlayerCharacter::Turn);
    PlayerInputComponent->BindAxis("lookup", this, &APlayerCharacter::LookUp);
}

void APlayerCharacter::Landed(const FHitResult& Hit)
{
    Super::Landed(Hit);

    ResetJump(MaxJumps);
}

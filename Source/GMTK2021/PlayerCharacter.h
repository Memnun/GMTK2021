// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "PlayerCharacter.generated.h"

class UPlayerMovement;
class UCameraComponent;
class USpringArmComponent;
class UBoxComponent;
class AController;

UENUM()
enum EWallRunSide {
	LEFT	UMETA(DisplayName = "Left"),
	RIGHT	UMETA(DisplayName = "Right")
};

UENUM()
enum EWallRunEndReason {
    FALL	UMETA(DisplayName = "Fell off wall"),
    JUMP	UMETA(DisplayName = "Jumped off wall")
};

UCLASS()
class GMTK2021_API APlayerCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    APlayerCharacter(const FObjectInitializer& ObjectInitializer);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = WallRun)
    int MaxJumps;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = WallRun)
    int WallJumpVelocity;

    bool bIsWallRunning;

private:

    FVector WallRunDirection;

    TEnumAsByte<EWallRunSide> WallRunSide;
    TEnumAsByte<EWallRunSide> PrevWallRunSide;

    FTimerHandle WallRunHandle;

    int JumpsRemaining;

    bool bIsSprinting;

    bool bWantsToWalk;

    float LastJumpTime;

    float LastJumpBoostTime;

    float MaxJumpTime;

    float ForwardAxis;
    float RightAxis;

    float CameraTargetRoll;

    UPlayerMovement* MovementPtr;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

    UFUNCTION()
    void MoveForward(float Value);

    UFUNCTION()
    void MoveRight(float Value);

    UFUNCTION()
    void Turn(float Rate);

    UFUNCTION()
    void LookUp(float Rate);

    UFUNCTION()
    void Sprint();

    UFUNCTION()
    void StopSprinting();

    UFUNCTION()
    void DoJump();

    UFUNCTION()
    void ResetJump(int Jumps);

    UFUNCTION()
    void BeginWallRun();

    UFUNCTION()
    void UpdateWallRun();

    UFUNCTION()
    void EndWallRun(EWallRunEndReason Reason);

    UFUNCTION()
    void BeginCameraTilt();

    UFUNCTION()
    void EndCameraTilt();

    UFUNCTION()
    void FindRunDirectionAndSide(FVector WallNormal);

    UFUNCTION()
    bool CanSurfaceBeWallRan(FVector SurfaceNormal) const;

    UFUNCTION()
    FVector FindLaunchVelocity();

    UFUNCTION()
    bool CheckRequiredKeys();

    UFUNCTION()
    bool ConsumeJump();

    UFUNCTION()
    void JumpBoost();

    UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Player|Camera")
    float BaseTurnRate;

    UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Player|Camera")
    float BaseLookUpRate;

    UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Player|Gameplay")
    bool bAutoBunnyhop;

public:

    UPROPERTY(EditAnywhere)
    UCameraComponent* CameraComponent;

    UPROPERTY(EditAnywhere)
    USpringArmComponent* SpringArmComponent;
    
    UFUNCTION()
    void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    virtual void OnJumped_Implementation() override;
    virtual bool CanJumpInternal_Implementation() const override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual void Landed(const FHitResult& Hit) override;

    float GetLastJumpTime()
    {
        return LastJumpTime;
    }

    UFUNCTION()
        bool IsSprinting() const
    {
        return bIsSprinting;
    }

    UFUNCTION()
        bool DoesWantToWalk() const
    {
        return bWantsToWalk;
    }

    UFUNCTION(Category = "B Getters", BlueprintPure) FORCEINLINE UPlayerMovement* GetMovementPtr() const
    {
        return MovementPtr;
    };

    /* Change Camera Height */
    void RecalculateBaseEyeHeight() override
    {
        Super::Super::RecalculateBaseEyeHeight();
    }
};

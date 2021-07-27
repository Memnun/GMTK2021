// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AttachmentPickup.h"

#include "PlayerCharacter.generated.h"

class UPlayerMovement;
class UCameraComponent;
class USpringArmComponent;
class UBoxComponent;
class AController;
class AWeapon;
class AAttachmentPickup;
class UHealthComponent;

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

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
    float ViewSwayAmount;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
    float ViewWeaponLeadAmount;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
    float MaxWeaponSway;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WallRun)
    int MaxJumps;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = WallRun)
    int WallJumpVelocity;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = WallRun)
    float WallRunClimbVelocity;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = WallRun)
    float WallRunFieldOfView;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
    float RollAngle;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
    float RollSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
    float BobSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
    float BobAmount;

    bool bIsWallRunning;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Controls)
    float MouseSensitivity;

    UPROPERTY(BlueprintReadWrite, Category = "Movement")
    bool bCanWallRun = false;

    UPROPERTY(BlueprintReadWrite, Category = "Movement")
    bool bCanSpeedBoost = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
    FVector WeaponOffset;

    UPROPERTY(EditAnywhere, Category = Spawn)
    TSubclassOf<AWeapon> SpawnWeapon;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Weapon")
    AWeapon* CurrentWeapon;

    bool bCanWallRun;

    bool bHasSpeedBoost;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attachments")
    TMap<EAttachmentClass, AAttachmentPickup*> Attachments;

private:

    float NormalFieldOfView;

    float TargetFieldOfView;

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

    UFUNCTION()
    float CalculateViewRoll();

    UFUNCTION()
    void CalculateWeaponSway(float DeltaTime);

    UFUNCTION()
    void FireWeapon();

    UFUNCTION()
    void StopFireWeapon();

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

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UHealthComponent* HealthComponent = nullptr;

    /*Changes the currently equipped weapon to a new weapon class, */
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
    AWeapon* SetCurrentWeaponByClass(TSubclassOf<AWeapon> NewWeaponClass);

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponChangedDelegate, AWeapon*, NewWeapon);
    UPROPERTY(BlueprintAssignable, BlueprintReadOnly)
    FOnWeaponChangedDelegate OnWeaponChanged;

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

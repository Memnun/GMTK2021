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

UCLASS()
class GMTK2021_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter(const FObjectInitializer& ObjectInitializer);

private:

	bool bIsSprinting;

	bool bWantsToWalk;

	float LastJumpTime;

	float LastJumpBoostTime;

	float MaxJumpTime;

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

	virtual void ClearJumpInput(float DeltaTime) override;
	virtual void OnJumped_Implementation() override;
	virtual bool CanJumpInternal_Implementation() const override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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

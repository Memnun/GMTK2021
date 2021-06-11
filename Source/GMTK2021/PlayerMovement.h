// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerMovement.generated.h"

/**
 *
 */
UCLASS()
class GMTK2021_API UPlayerMovement : public UCharacterMovementComponent
{
	GENERATED_BODY()

protected:

	/** Milliseconds between step sounds */
	float MoveSoundTime;

	/** If we are stepping left, else, right */
	bool StepSide;

	/** The multiplier for acceleration when on ground. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Walking")
	float GroundAccelerationMultiplier;

	/** The multiplier for acceleration when in air. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Walking")
	float AirAccelerationMultiplier;

	/* The vector differential magnitude cap when in air. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Jumping / Falling")
	float AirSpeedCap;

	/** Time to crouch on ground in seconds */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Walking")
	float CrouchTime;

	/** Time to uncrouch on ground in seconds */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Walking")
	float UncrouchTime;

	/** Time to crouch in air in seconds */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Walking")
	float CrouchJumpTime;

	/** Time to uncrouch in air in seconds */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Walking")
	float UncrouchJumpTime;

	/** the minimum step height from moving fast */
	UPROPERTY(Category = "Character Movement: Walking", EditAnywhere, BlueprintReadWrite)
	float MinStepHeight;

	/** If the player has already landed for a frame, and breaking may be applied. */
	bool bBrakingFrameTolerated;

	/** If in the crouching transition */
	bool bInCrouch;

	/** The PB player character */
	class APBPlayerCharacter* PBCharacter;

	/** The target ground speed when running. */
	UPROPERTY(Category = "Character Movement: Walking", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	float RunSpeed;

	/** The target ground speed when sprinting.  */
	UPROPERTY(Category = "Character Movement: Walking", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	float SprintSpeed;

	/** The target ground speed when walking slowly. */
	UPROPERTY(Category = "Character Movement: Walking", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	float WalkSpeed;

	/** The minimum speed to scale up from for slope movement  */
	UPROPERTY(Category = "Character Movement: Walking", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	float SpeedMultMin;

	/** The maximum speed to scale up to for slope movement */
	UPROPERTY(Category = "Character Movement: Walking", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	float SpeedMultMax;

	bool bAppliedFriction;

	class APlayerCharacter* Character;

public:

	/** Print pos and vel (Source: cl_showpos) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement (General Settings)")
	uint32 bShowPos : 1;

	UPlayerMovement();

	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void CalcVelocity(float DeltaTime, float Friction, bool bFluid, float BrakingDeceleration) override;
	virtual void ApplyVelocityBraking(float DeltaTime, float Friction, float BrakingDeceleration) override;

	virtual float GetMaxSpeed() const override;

	bool IsBrakingFrameTolerated() const
	{
		return bBrakingFrameTolerated;
	}
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AIFlying.generated.h"


UCLASS()
class GMTK2021_API AAIFlying : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAIFlying(const FObjectInitializer& ObjectInitializer);

	class UFloatingPawnMovement* MovementPtr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveTowardsPlayer();

public:	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIFlying.generated.h"

UCLASS()
class GMTK2021_API AAIFlying : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAIFlying();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = AI)
    class UBehaviorTree* BehaviorTree;
};

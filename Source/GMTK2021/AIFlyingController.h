// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIFlyingController.generated.h"

/**
 * 
 */
UCLASS()
class GMTK2021_API AAIFlyingController : public AAIController
{
	GENERATED_BODY()

    UBlackboardComponent* BlackboardComponent;
    class UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(EditDefaultsOnly, Category = AI)
    FName Key_LocationToGo;

	UPROPERTY(EditDefaultsOnly, Category = AI)
    FName Key_PlayerLocation;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

public:
	AAIFlyingController();

	// Getters
	FORCEINLINE UBlackboardComponent* GetBlackboardComponent() const { return BlackboardComponent; }
};

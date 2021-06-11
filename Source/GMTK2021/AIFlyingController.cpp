// Fill out your copyright notice in the Description page of Project Settings.


#include "AIFlyingController.h"
#include "AIFlying.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"

AAIFlyingController::AAIFlyingController()
{

}

void AAIFlyingController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
}

void AAIFlyingController::OnUnPossess()
{
    Super::OnUnPossess();
}

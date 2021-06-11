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

    BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComponent"));
    BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

    Key_LocationToGo = "LocationToGo";
    Key_PlayerLocation = "Target";

}

void AAIFlyingController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    AAIFlying* AICharacter = Cast<AAIFlying>(InPawn);

    if (AICharacter)
    {
        if (AICharacter->BehaviorTree->BlackboardAsset)
        {
            BlackboardComponent->InitializeBlackboard(*(AICharacter->BehaviorTree->BlackboardAsset));
        }

        BehaviorTreeComponent->StartTree(*AICharacter->BehaviorTree);
    }
}

void AAIFlyingController::OnUnPossess()
{
    Super::OnUnPossess();

    BehaviorTreeComponent->StopTree();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "AIFlying.h"

#include "GameFramework/FloatingPawnMovement.h"
#include "Components/SphereComponent.h"

// Sets default values
AAIFlying::AAIFlying(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<UFloatingPawnmovement>(ACharacter::CharacterMovementComponentName))
{

    MovementPtr = Cast<UFloatingPawnMovement>(ACharacter::GetMovementComponent());
}

// Called when the game starts or when spawned
void AAIFlying::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle FindPlayerHandle;
	GetWorld()->GetTimerManager().SetTimer(FindPlayerHandle, this, &AAIFlying::MoveTowardsPlayer, 0.5f, true);
}

void AAIFlying::MoveTowardsPlayer()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("Yeah"));

	//Cast<UFloatingPawnMovement>(CharacterMovement)
}


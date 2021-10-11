// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupBase.h"
#include "Components/PrimitiveComponent.h"
#include "PlayerCharacter.h"

// Sets default values
APickupBase::APickupBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(RootMesh);
	RootMesh->SetSimulatePhysics(true);
	RootMesh->SetCollisionProfileName("Pickup");

	PickupRadius = CreateDefaultSubobject<USphereComponent>("PickupRadius");
	PickupRadius->SetupAttachment(RootComponent);
	PickupRadius->OnComponentBeginOverlap.AddDynamic(this, &APickupBase::OnPickupOverlap);
}

// Called when the game starts or when spawned
void APickupBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickupBase::BeginApplyPickup()
{
	BeginApply_Internal();
	ApplyPickupEvent();
	PickupRadius->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetActorHiddenInGame(true);

	if (bPersistant)
	{
		if (Duration > 0)
			GetWorld()->GetTimerManager().SetTimer(ExpirationTimer, this, &APickupBase::OnEffectEnd_Internal, Duration, false);
	}
	else
	{
		SetLifeSpan(.1f);
	}
}

void APickupBase::OnEffectEnd_Internal()
{
	OnEffectEnd();
	SetLifeSpan(.1f);
}

void APickupBase::OnPickupOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Player = Cast<APlayerCharacter>(OtherActor);
	if (!Player) return;

	if (bAutoPickup)
	{
		BeginApplyPickup();
	}
	else
	{
		DeferredPickup();
	}
}


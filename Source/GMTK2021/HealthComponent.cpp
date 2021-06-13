// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
    DefaultHealth = 100;
    Health = DefaultHealth;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    AActor* Owner = GetOwner();
    if (Owner)
    {
        Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
    }
}


// Called every frame
void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
    if (Damage < 0)
        return;

    Health = FMath::Clamp<float>(Health - Damage, 0.f, DefaultHealth);
}


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GMTK2021_API UHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UHealthComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
    float DefaultHealth;

    UPROPERTY(BlueprintReadOnly)
    float Health;

    UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Category = Health)
    float Armor = 0.f;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathDelegate);
    UPROPERTY(BlueprintAssignable, Category = "Health")
    FOnDeathDelegate OnDeath;

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

    UFUNCTION()
    void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

public:

};

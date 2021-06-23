// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "PickupBase.generated.h"

class APlayerCharacter;

UCLASS(Abstract)
class GMTK2021_API APickupBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickupBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/********************************* SETTINGS *****************************************/

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Mesh")
		UStaticMeshComponent* RootMesh = nullptr;

	/*Volume player has to enter to be able to pickup the */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Collision")
		USphereComponent* PickupRadius = nullptr;

	/*Whether the pickup is automatically picked up or if the pickup has to be triggered by player action*/
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Pickup")
		bool bAutoPickup = true;

	/*Whether or not this is an effect that has to be removed later*/
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Pickup")
		bool bPersistant = true;

	/* If higher than 0 for a persistent effect, will automatically remove the effect after this value in seconds*/
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Pickup")
		float Duration = -1.f;

	/*********************************** Effect Implementation ************************************/

	UFUNCTION(BlueprintCallable, Category = "Pickup")
		void BeginApplyPickup();

	UFUNCTION(BlueprintCallable, Category = "Pickup")
		void ForceEndEffect() { OnEffectEnd_Internal(); }

	/*Overrideable event called when the object is picked up by a player, used to apply the effects*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Pickup")
		void ApplyPickupEvent();

	/*Overrideable event called when this pickup's effect end, if it's not a one-shot effect and has a duration*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Pickup")
		void OnEffectEnd();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Pickup")
		APlayerCharacter* Player = nullptr;

	virtual void DeferredPickup() {}
private:
	FTimerHandle ExpirationTimer;

	UFUNCTION() void OnEffectEnd_Internal();

	UFUNCTION()
		void OnPickupOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};

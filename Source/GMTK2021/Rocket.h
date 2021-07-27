// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Rocket.generated.h"

UCLASS()
class GMTK2021_API ARocket : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARocket();

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
    class USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
    class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
    class UStaticMeshComponent* MeshComponent;

	UFUNCTION()
    void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	float DamageMultiplier = 1.f;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

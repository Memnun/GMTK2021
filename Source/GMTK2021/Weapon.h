// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
namespace EWeaponProjectile
{
    enum ProjectileType
    {
        EHitScan	UMETA(DisplayName = "Hit scan"),
        EProjectile	UMETA(DisplayName = "Projectile")
    };
}

USTRUCT()
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, Category = Config)
    float TimeBetweenShots;

    UPROPERTY(EditDefaultsOnly, Category = Config)
    bool AutoFire;
};

UCLASS()
class GMTK2021_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	UFUNCTION()
    void Fire();

	UFUNCTION()
    void StopFire();

	UFUNCTION()
    void HitScanFire();

	UFUNCTION()
    void ProjectileFire();

	UPROPERTY(EditAnywhere, Category = Config)
	FWeaponData WeaponConfig;

	UPROPERTY(EditAnywhere, Category = Config)
	TEnumAsByte<EWeaponProjectile::ProjectileType> ProjectileType;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* WeaponMesh;

    UPROPERTY(EditDefaultsOnly, Category = Projectile)
    TSubclassOf<class ARocket> RocketProjectile;

protected:

	UFUNCTION()
    void ResetFire();

	bool bCanFire;
	bool bWantsToFire;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

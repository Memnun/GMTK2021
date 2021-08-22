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

// XXX This doesn't really need to exist as a struct does it?
USTRUCT()
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, Category = Config)
    float TimeBetweenShots;

    UPROPERTY(EditDefaultsOnly, Category = Config)
    float InFireDelay;

    UPROPERTY(EditDefaultsOnly, Category = Config)
    bool AutoFire;

    UPROPERTY(EditDefaultsOnly, Category = Config)
    int32 MaxAmmo;

    UPROPERTY(EditDefaultsOnly, Category = Config)
    float Damage = 10.f;
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
    void TryFire();

	UFUNCTION(BlueprintImplementableEvent, Category = Weapon)
    void OnFireEvent();

	UFUNCTION(BlueprintImplementableEvent, Category = Weapon)
    void OnStopFireEvent();

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

	UPROPERTY(BlueprintReadOnly, Category = Weapon)
	int32 CurrentAmmo;

	UPROPERTY(BlueprintReadWrite, Category = Weapon)
	float DamageMultiplier = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
    FVector WeaponOffset;

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

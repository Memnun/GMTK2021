// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(BoxComponent);

}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::Fire()
{
	switch (ProjectileType)
	{
	case EWeaponProjectile::ProjectileType::EHitScan:
		HitScanFire();
		break;
	case EWeaponProjectile::ProjectileType::EProjectile:
		break;
	}
}

void AWeapon::HitScanFire()
{
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, TEXT("Hit scan!"));
}

void AWeapon::ProjectileFire()
{
}

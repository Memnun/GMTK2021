// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(BoxComponent);

    bCanFire = true;
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
    if (bCanFire)
    {
        bWantsToFire = true;

        switch (ProjectileType)
        {
        case EWeaponProjectile::ProjectileType::EHitScan:
            HitScanFire();
            break;
        case EWeaponProjectile::ProjectileType::EProjectile:
            break;
        }

        FTimerHandle ShootDelayHandle;
        GetWorld()->GetTimerManager().SetTimer(ShootDelayHandle, this, &AWeapon::ResetFire, WeaponConfig.TimeBetweenShots);
        bCanFire = false;
        
    }
}

void AWeapon::HitScanFire()
{
    FVector Start;
    FVector End;

    FVector Location;
    FRotator Rotation;
    FHitResult Hit;

    GetInstigator()->GetController()->GetPlayerViewPoint(Location, Rotation);

    Start = Location;
    End = Start + (Rotation.Vector() * 10000);

    FCollisionQueryParams TraceParams;
    GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);

    DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 5.0f);

    GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, TEXT("Hit scan!"));
}

void AWeapon::ProjectileFire()
{
}

void AWeapon::StopFire()
{
    bWantsToFire = false;
}

void AWeapon::ResetFire()
{
    bCanFire = true;
    if (WeaponConfig.AutoFire && bWantsToFire)
    {
        Fire();
    }
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Rocket.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(BoxComponent);

    WeaponOffset = FVector(20.f, 30.f, -40.f);

    bCanFire = true;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
    CurrentAmmo = WeaponConfig.MaxAmmo;
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
        ProjectileFire();
        break;
    }

    CurrentAmmo--;

    FTimerHandle ShootDelayHandle;
    GetWorld()->GetTimerManager().SetTimer(ShootDelayHandle, this, &AWeapon::ResetFire, WeaponConfig.TimeBetweenShots);

    OnFireEvent();
}

void AWeapon::TryFire()
{
    if (bCanFire && CurrentAmmo > 0)
    {
        bWantsToFire = true;

        if (WeaponConfig.InFireDelay > 0)
        {
            // Delayed fire
            FTimerHandle DelayedShootHandle;
            GetWorld()->GetTimerManager().SetTimer(DelayedShootHandle, this, &AWeapon::Fire, WeaponConfig.InFireDelay);
        }
        else
        {
            // Instant fire
            Fire();
        }


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

    if (Hit.bBlockingHit && Hit.GetActor())
    {
        TSubclassOf<UDamageType> DamageType;
        UGameplayStatics::ApplyDamage(Hit.GetActor(), WeaponConfig.Damage * DamageMultiplier, GetInstigator()->GetController(), GetInstigator(), DamageType);
    }
    //DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 5.0f);
}

void AWeapon::ProjectileFire()
{
    FVector Location = WeaponMesh->GetSocketLocation(TEXT("Muzzle"));
    FRotator Rotation = WeaponMesh->GetSocketRotation(TEXT("Muzzle"));

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.Instigator = GetInstigator();
    const ARocket* SpawnedRocket = GetWorld()->SpawnActor<ARocket>(RocketProjectile, Location, Rotation, SpawnParams);

    if (SpawnedRocket)
    {

    }
}

void AWeapon::StopFire()
{
    bWantsToFire = false;
    OnStopFireEvent();
}

void AWeapon::ResetFire()
{
    bCanFire = true;
    if (WeaponConfig.AutoFire && bWantsToFire)
    {
        Fire();
    }
}

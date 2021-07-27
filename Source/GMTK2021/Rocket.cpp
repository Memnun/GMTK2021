// Fill out your copyright notice in the Description page of Project Settings.


#include "Rocket.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
ARocket::ARocket()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	SphereComponent->OnComponentHit.AddDynamic(this, &ARocket::OnComponentHit);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = SphereComponent;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	MeshComponent->SetupAttachment(SphereComponent);
	
	InitialLifeSpan = 5.f;
}

void ARocket::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != NULL && OtherActor != this && OtherComp != NULL)
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.f, GetActorLocation());

        TSubclassOf<UDamageType> DamageType;
		TArray<AActor*> IgnoredActors;
        UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(), 50.f * DamageMultiplier, 1, GetActorLocation(), 0.f, 250.f, 5.f, DamageType, IgnoredActors);
		DrawDebugSphere(GetWorld(), GetActorLocation(), 250.f, 32, FColor::Red, false, 4);
		Destroy();
	}
}

// Called when the game starts or when spawned
void ARocket::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


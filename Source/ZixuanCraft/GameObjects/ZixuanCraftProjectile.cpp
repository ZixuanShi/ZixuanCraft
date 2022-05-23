// Copyright Epic Games, Inc. All Rights Reserved.

#include "ZixuanCraftProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

AZixuanCraftProjectile::AZixuanCraftProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(10.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AZixuanCraftProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 10000.f;
	ProjectileMovement->MaxSpeed = 10000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	// Bullet body mesh component 
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereFinder(TEXT("/Engine/BasicShapes/Sphere"));
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	BodyMesh->SetupAttachment(RootComponent);
	BodyMesh->SetStaticMesh(SphereFinder.Object);
	BodyMesh->SetWorldScale3D(FVector(0.05f, 0.05f, 0.05f));
	BodyMesh->SetCollisionProfileName(TEXT("NoCollision"));	// Collision component takes care collision stuff

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void AZixuanCraftProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// If the other actor is a spawnable, apply damage
	if (OtherActor && (OtherActor != this) && (OtherComp != nullptr))
	{
		if (OtherActor != GetOwner())
		{
			UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigator()->GetController(), GetOwner(), UDamageType::StaticClass());
		}
		if (OtherComp->IsSimulatingPhysics())
		{
			OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
		}

		Destroy();
	}
}
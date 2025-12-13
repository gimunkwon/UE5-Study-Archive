// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie_Projectile.h"

#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include <Kismet/GameplayStatics.h>
#include "Particles/ParticleSystem.h"


AZombie_Projectile::AZombie_Projectile()
{

	PrimaryActorTick.bCanEverTick = false; 

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->InitSphereRadius(15.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("Projectile")); 
	RootComponent = CollisionComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(CollisionComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision); 

	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
	ProjectileMovement->UpdatedComponent = CollisionComponent;
	ProjectileMovement->InitialSpeed = 2000.f; 
	ProjectileMovement->MaxSpeed = 2000.f; 
	ProjectileMovement->bRotationFollowsVelocity = true; 
	ProjectileMovement->bShouldBounce = false; 

	
	InitialLifeSpan = 3.0f;


	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AZombie_Projectile::OnOverlapBegin);

}


void AZombie_Projectile::BeginPlay()
{
	Super::BeginPlay();
	
	// 내 주인이 있다면
	if (GetOwner())
	{
		CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
	}
	
}


void AZombie_Projectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (OtherActor && (OtherActor != this) && (OtherActor != GetOwner()))
	{
		
		UGameplayStatics::ApplyDamage(OtherActor, 20.0f, GetInstigatorController(), this, UDamageType::StaticClass());
		
		// 타격 이펙트 재생
		if (HitEffect)
		{
			// 부딪힌 위치에 이펙트 생성
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, GetActorLocation(),GetActorRotation(),FVector(1.0f));
		}
		
		Destroy();
	}
}

void AZombie_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Zombie_Projectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystem;

UCLASS()
class ZOMBIESURVIVOR_API AZombie_Projectile : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AZombie_Projectile();

protected:
	virtual void BeginPlay() override;

	
	UPROPERTY(VisibleAnywhere, Category = "Projectile")
	USphereComponent* CollisionComponent;

	
	UPROPERTY(VisibleAnywhere, Category = "Projectile")
	UStaticMeshComponent* MeshComponent;

	
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovement;
	
	// 타격 이펙트
	UPROPERTY(EditDefaultsOnly, Category="Combat")
	UParticleSystem* HitEffect;
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	
	virtual void Tick(float DeltaTime) override;

};

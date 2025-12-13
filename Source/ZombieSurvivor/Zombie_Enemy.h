// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Zombie_Enemy.generated.h"


class UWidgetComponent;
class UZombie_HUD;


UCLASS()
class ZOMBIESURVIVOR_API AZombie_Enemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZombie_Enemy();

	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInsigator, AActor* DamageCauser) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float MaxHealth = 100.0f;

	float CurrentHealth;

	// UI를 3D 공간에 띄워주는 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* HealthWidgetComponent;
	
	// 공격용 타이머
	FTimerHandle AttackTimerHandle;
	
	// 공격 함수
	void Attack();
	
	// 겹침 시작/종료 감지 (액터의 기본 함수 오버라이드)
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	
	// 공격 범위를 감지할 센서
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
	class USphereComponent* AttackRangeSphere;
	// 센서에 닿았을 때 실행할 함수
	UFUNCTION()
	void OnAttackRangeOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	// 센서에 나갔을 때 실행할 함수
	UFUNCTION()
	void OnAttackRangeEndlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

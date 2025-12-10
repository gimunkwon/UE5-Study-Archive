// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Dungeon_Enemy.generated.h"

class UWidgetComponent;
class ADungeon_DamageTextActor;

UCLASS()
class DUNGEON_CRAWLER_API ADungeon_Enemy : public ACharacter
{
	GENERATED_BODY()

public:
	ADungeon_Enemy();
protected:
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
public:
	// 데미지 받는 함수 (AActor 오버라이드)
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
protected:
	// 스탯
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	float MaxHealth = 100.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Stats")
	float CurrentHealth;
	//데미지 폰트 액터 설계도
	UPROPERTY(EditDefaultsOnly, Category= "UI")
	TSubclassOf<ADungeon_DamageTextActor> DamageTextClass;
	// 머리 위 체력바 컴포넌트
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category="UI")
	UWidgetComponent* HealthWidgetComp;
	
	// 공격용 타이머
	FTimerHandle AttackTimerHandle;
	// 실제 데미지를 주는 함수
	void AttackCheck();
	// 겹침 시작/종료 감지 (액터의 기본 함수 오버라이드)
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	
	
	// 사망처리 함수
	void Die();

};

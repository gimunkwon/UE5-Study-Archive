// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Zombie_GameMode.generated.h"

class AZombie_Enemy;

UCLASS()
class ZOMBIESURVIVOR_API AZombie_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AZombie_GameMode();
	
protected:
	virtual void BeginPlay() override;
	
	// 스폰 설정 변수
	// 1. 소환할 좀비의 설계도 (에디터에서 BP_Zombie를 넣을 곳)
	UPROPERTY(EditDefaultsOnly, Category= "Wave")
	TSubclassOf<AZombie_Enemy> ZombieClass;
	
	// 2. 스폰 타이머 핸들
	FTimerHandle SpawnTimerHandle;
	
	// 3. 몇 초마다 스폰할지
	UPROPERTY(EditDefaultsOnly, Category= "Wave")
	float SpawnInterval = 2.0f;
	
	// 4. 실제 스폰을 수행할 함수
	void SpawnZombie();
	
};

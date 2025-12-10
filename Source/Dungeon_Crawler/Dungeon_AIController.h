// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Dungeon_AIController.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEON_CRAWLER_API ADungeon_AIController : public AAIController
{
	GENERATED_BODY()
	
protected:
	// 게임 시작 시 실행
	virtual void BeginPlay() override;
	
public:
	// 매 프레임마다 실행 (감시 및 추적)
	virtual void Tick(float DeltaSeconds) override;
	
};

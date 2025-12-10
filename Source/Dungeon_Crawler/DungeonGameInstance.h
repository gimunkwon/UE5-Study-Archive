// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DungeonGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEON_CRAWLER_API UDungeonGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	// 맵이 바뀌어도 유지될 퀘스트 데이터
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Quest Data")
	bool bHasQuest = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Quest Data")
	bool bIsCompleted = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Quest Data")
	int32 CurrentKills = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Quest Data")
	int32 TargetKills = 3;
};

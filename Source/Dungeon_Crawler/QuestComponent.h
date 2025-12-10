// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Dungeon_ItemStruct.h"
#include "QuestComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DUNGEON_CRAWLER_API UQuestComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UQuestComponent();
	
	// 퀘스트 상태 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Quest")
	bool bHasQuest = false; // 퀘스트를 받았는가?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Quest")
	bool bIsCompleted = false; // 보상까지 다 받았는가?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Quest")
	int32 CurrentKills = 3; // 현재 잡은수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Quest")
	int32 TargetKills = 0; // 목표수(3마리)
	
	// 함수
	// 퀘스트 시작
	void StartQuest();
	//적을 잡았을때 호출됨
	void OnEnemykilled();
	// 퀘스트 완료 처리 (보상 받음)
	void FinishQuest();
	
	void SaveToGameInstance();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};

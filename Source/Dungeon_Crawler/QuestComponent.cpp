// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestComponent.h"
#include "DungeonGameInstance.h"
#include "Kismet/GameplayStatics.h"

#include "Engine/World.h"

// Sets default values for this component's properties
UQuestComponent::UQuestComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UQuestComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// 1. 현재 게임 인스턴스 가져오기
	UGameInstance* BaseGI = UGameplayStatics::GetGameInstance(GetWorld());

	if (!BaseGI) return;

	// ★★★ [핵심] 현재 적용된 GameInstance 클래스 이름 확인 ★★★
	// 로그에 "GameInstance"라고 뜨면 설정 실패! "DungeonGameInstance"라고 떠야 성공!
	UE_LOG(LogTemp, Error, TEXT("🔎 현재 적용된 GI 클래스: %s"), *BaseGI->GetClass()->GetName());
	
	// 금고(GameInstance) 가져오기
	UDungeonGameInstance* GI = Cast<UDungeonGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	if (GI)
	{
		UE_LOG(LogTemp, Error, TEXT("현재 사용중인 GameInstance 클래스 :%s"), *GI->GetClass()->GetName());
		// 금고에 있는 데이터를 내 변수로 복사 (동기화)
		bHasQuest = GI->bHasQuest;
		bIsCompleted = GI->bIsCompleted;
		CurrentKills = GI->CurrentKills;
		TargetKills = GI->TargetKills;
		
		UE_LOG(LogTemp, Warning, TEXT("퀘스트 데이터 로드 성공! 현재 킬 수:%d / 퀘스트 보유:%s"),CurrentKills,bHasQuest ? TEXT("O") : TEXT("X"));
		
	}
}

void UQuestComponent::StartQuest()
{
	if (bIsCompleted) return; // 이미 깬 퀘스트면 무시
	bHasQuest = true;
	SaveToGameInstance();
	UE_LOG(LogTemp, Warning, TEXT("퀘스트 시작: 적 3마리를 처치하세요!"));
	UE_LOG(LogTemp, Warning, TEXT("퀘스트 데이터 로드 성공! 현재 킬 수:%d / 퀘스트 보유:%s"),CurrentKills,bHasQuest ? TEXT("O") : TEXT("X"));
}

void UQuestComponent::OnEnemykilled()
{
	// 퀘스트가 있고, 아직 목표를 달성 못했다면
	if (bHasQuest && !bIsCompleted && CurrentKills < TargetKills)
	{
		CurrentKills++;
		
		SaveToGameInstance();
		
		UE_LOG(LogTemp, Warning, TEXT("적 처치! (%d, %d)"),CurrentKills, TargetKills);
		
		if (CurrentKills >= TargetKills)
		{
			UE_LOG(LogTemp, Warning, TEXT("목표 달성! 촌장에게 돌아가세요."));
		}
	}
}

void UQuestComponent::FinishQuest()
{
	bHasQuest = false;
	bIsCompleted = true;
	
	SaveToGameInstance();
	
	UE_LOG(LogTemp, Warning, TEXT("퀘스트 완료 보상 지급됨"));
}

void UQuestComponent::SaveToGameInstance()
{
	UDungeonGameInstance* GI = Cast<UDungeonGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	if (GI)
	{
		UE_LOG(LogTemp,Warning,TEXT("퀘스트 동기화 시작"))
		// 금고에 있는 데이터를 내 변수로 복사 (동기화)
		GI->bHasQuest = bHasQuest;
		GI->bIsCompleted = bIsCompleted;
		GI->CurrentKills = CurrentKills;
		GI->TargetKills = TargetKills;
		
		// UE_LOG(LogTemp, Warning, TEXT("저장완료 금고에 넣은 값 -> Quest: %s"), bHasQuest ? TEXT("O") : TEXT("X"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("게임 인스터스를 찾을수 없습니다."));
	}
}




// Called every frame
void UQuestComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 0.0f, FColor::Yellow
			, FString::Printf(TEXT("현재 퀘스트 보유 상태 %s"),bHasQuest ? TEXT("True"):TEXT("False")));
	}
	
}


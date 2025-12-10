// Fill out your copyright notice in the Description page of Project Settings.


#include "Dungeon_AIController.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

void ADungeon_AIController::BeginPlay()
{
	Super::BeginPlay();
	// 나중에 여기서 비헤이비어 트리 실행 로직 등을 넣을 수 있음
}


void ADungeon_AIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	// 1. 플레이어 찾기
	// 멀티플레이어가 아니면 Index 0이 무조건 내 캐릭터
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	
	if (PlayerPawn)
	{
		// 2. 플레이어 에게 이동 명령 내리기
		// - AcceptanceRadius : 200cm 앞까지 가서 멈춰라
		// - StopOnOverlap: 목표물에 닿으면 멈출지 여부
		MoveToActor(PlayerPawn, 200.0f);
	}
}

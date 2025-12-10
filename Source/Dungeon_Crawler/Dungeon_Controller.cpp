// Fill out your copyright notice in the Description page of Project Settings.


#include "Dungeon_Controller.h"

ADungeon_Controller::ADungeon_Controller()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}

void ADungeon_Controller::BeginPlay()
{
	Super::BeginPlay();
	
	// 1. 입력모드를 [게임 + UI혼합]으로 설정
	// 그래야 캐릭터도 움직이고, 마우스로 UI도 누를 수 있음
	FInputModeGameAndUI InputMode;
	
	// 2. 마우스가 게임화면 밖으로 탈출 못하게 가두기
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	
	// 3. 클릭할때 커서 사라지는 거 방지
	// 이걸 안하면 클릭할 때마다 커서가 깜빡거리거나 사라져서 연타하기 힘듬
	InputMode.SetHideCursorDuringCapture(false);
	
	// 설정 적용
	SetInputMode(InputMode);
}

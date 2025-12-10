// Fill out your copyright notice in the Description page of Project Settings.


#include "Dungeon_DialogueWidget.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

void UDungeon_DialogueWidget::UpdateDialogue(FString Name, FString Content)
{
	// 텍스트 변경
	if (Text_Name)
	{
		Text_Name->SetText(FText::FromString(Name));
	}
	if (Text_Content)
	{
		Text_Content->SetText(FText::FromString(Content));
	}
}

void UDungeon_DialogueWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// 버튼에 함수 연결
	if (Btn_Close)
	{
		Btn_Close->OnClicked.AddDynamic(this, &UDungeon_DialogueWidget::OnCloseClicked);
	}
}

void UDungeon_DialogueWidget::OnCloseClicked()
{
	// 1. 위젯 닫기 (화면에서 제거)
	RemoveFromParent();
	
	// 2. 플레이어 컨트롤러 가져오기
	if (APlayerController* PC = GetOwningPlayer())
	{
		// 3. 인풋 모드를 다시 게임으로 복구
		PC->SetInputMode(FInputModeGameAndUI());
	}
	UE_LOG(LogTemp, Warning, TEXT("대화종료"));
}

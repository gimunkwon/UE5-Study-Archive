// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Dungeon_DialogueWidget.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEON_CRAWLER_API UDungeon_DialogueWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	// 대화 내용 업데이트 함수
	void UpdateDialogue(FString Name, FString Content);
	virtual void NativeConstruct() override;
	
protected:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* Text_Name; // 화자 이름
	UPROPERTY(meta=(BindWidget))
	UTextBlock* Text_Content; // 대화 내용
	// 닫기 버튼
	UPROPERTY(meta=(BindWidget))
	UButton* Btn_Close;
private:
	// 버튼 클릭시 실행할 함수
	UFUNCTION()
	void OnCloseClicked();
};

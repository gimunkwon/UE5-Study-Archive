// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Zombie_GameOverWidget.generated.h"

class UButton;

UCLASS()
class ZOMBIESURVIVOR_API UZombie_GameOverWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	// 재시작 버튼
	UPROPERTY(meta=(BindWidget))
	UButton* RestartBtn;
	
	// 메인 메뉴로 가는 버튼
	// UPROPERTY(meta=(BindWidget))
	// UButton* MainMenuBtn;
	
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnRestartClicked();
	
	UFUNCTION()
	void OnMainMenuClicked();
};

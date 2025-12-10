// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Dungeon_StatWidget.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class DUNGEON_CRAWLER_API UDungeon_StatWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// 체력 업데이트
	void UpdateHealth(float Current, float Max);
	
	// 마나 업데이트
	void UpdateMana(float Current, float Max);
	
protected:
	// 체력바 (필수)
	UPROPERTY(meta=(BindWidget))
	UProgressBar* HealthBar;
	
	// 마나바(선택 - 몬스터는 이게 없어도 에러 안남)
	UPROPERTY(meta=(BindWidgetOptional))
	UProgressBar* ManaBar;
	
};

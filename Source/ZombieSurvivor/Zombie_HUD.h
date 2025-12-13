// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Zombie_HUD.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class ZOMBIESURVIVOR_API UZombie_HUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// 외부(캐릭터)에서 이 함수를 불러서 게이지를 갱신함
	void UpdateHealth(float CurrentHealth, float MaxHealth);
	// 탄약 갱신 함수 추가
	void UpdateAmmo(int32 CurrentAmmo, int32 MaxAmmo);
	
protected:
	// 에디터의 프로그레스 바와 연결할 변수
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
	
	// 탄약 테스트
	UPROPERTY(meta=(BindWidgetOptional))
	UTextBlock* AmmoText;
	
};

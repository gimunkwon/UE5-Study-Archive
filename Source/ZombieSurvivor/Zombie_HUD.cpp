// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie_HUD.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UZombie_HUD::UpdateHealth(float CurrentHealth, float MaxHealth)
{
	if (HealthBar)
	{
		// 0 으로 나누기 방지
		if (MaxHealth <= 0.0f) return;
		
		// 퍼센트 계산 (예: 80 / 100 = 0.8)
		float Percent = CurrentHealth / MaxHealth;
		
		// 게이지 채우기
		HealthBar->SetPercent(Percent);
	}
}

void UZombie_HUD::UpdateAmmo(int32 CurrentAmmo, int32 MaxAmmo)
{
	if (AmmoText)
	{
		// 현재 / 최대 형식으로 문자열 만들기
		// 예 : 29 / 30
		FString AmmoString = FString::Printf(TEXT("%d / %d"), CurrentAmmo, MaxAmmo);
		
		// 텍스트 변경
		AmmoText->SetText(FText::FromString(AmmoString));
	}
	
	
}

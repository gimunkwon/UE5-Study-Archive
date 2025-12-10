// Fill out your copyright notice in the Description page of Project Settings.


#include "Dungeon_DamageTextWidget.h"

#include "Components/TextBlock.h"

void UDungeon_DamageTextWidget::SetDamageText(float Damage)
{
	if (DamageText)
	{
		// 정수(int)로 반올림해서 표시
		DamageText->SetText(FText::AsNumber(FMath::RoundToInt(Damage)));
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Dungeon_StatWidget.h"

#include "Components/ProgressBar.h"

void UDungeon_StatWidget::UpdateHealth(float Current, float Max)
{
	if (HealthBar && Max > 0.0f)
	{
		HealthBar->SetPercent(Current / Max);
	}
}

void UDungeon_StatWidget::UpdateMana(float Current, float Max)
{
	if (ManaBar && Max > 0.0f)
	{
		ManaBar->SetPercent(Current / Max);
	}
	
}

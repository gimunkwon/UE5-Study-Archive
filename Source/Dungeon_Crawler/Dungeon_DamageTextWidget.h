// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Dungeon_DamageTextWidget.generated.h"


class UTextBlock;

UCLASS()
class DUNGEON_CRAWLER_API UDungeon_DamageTextWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// 외부에서 데미지 값을 넣어주는 함수
	void SetDamageText(float Damage);
	
protected:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* DamageText;
};

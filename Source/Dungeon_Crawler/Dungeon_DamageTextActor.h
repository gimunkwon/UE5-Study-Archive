// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Dungeon_DamageTextActor.generated.h"

class UWidgetComponent;

UCLASS()
class DUNGEON_CRAWLER_API ADungeon_DamageTextActor : public AActor
{
	GENERATED_BODY()

public:
	ADungeon_DamageTextActor();
	virtual void Tick(float DeltaTime) override;
	
	// 데미지 설정 함수 (스폰 직후 호출)
	void SetDamage(float DamageAmount);
	

protected:
	virtual void BeginPlay() override;
	
	// 위젯을 보여줄 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWidgetComponent* DamageWidgetComp;
	
	// 위로 떠오로는 속도
	UPROPERTY(EditDefaultsOnly, Category="Effect")
	float FloatSpeed = 100.0f;
	
	
};

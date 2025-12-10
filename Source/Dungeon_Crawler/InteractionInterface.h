// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DUNGEON_CRAWLER_API IInteractionInterface
{
	GENERATED_BODY()

public:
	// 상호작용 함수
	// BlueprintNativeEvent : C++에서도 쓰고 블루프린터에서도 쓸수있게함
	// Instigator : 누가 눌렀는지 (플레이어)
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction")
	void Interact(APawn* InstigatorPawn);
};

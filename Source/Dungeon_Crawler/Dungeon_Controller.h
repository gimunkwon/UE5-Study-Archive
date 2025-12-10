// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Dungeon_Controller.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEON_CRAWLER_API ADungeon_Controller : public APlayerController
{
	GENERATED_BODY()
public:
	ADungeon_Controller();
	
	
protected:
	virtual void BeginPlay() override;
	
};

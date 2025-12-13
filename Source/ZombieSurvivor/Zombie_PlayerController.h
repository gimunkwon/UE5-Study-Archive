// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Zombie_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESURVIVOR_API AZombie_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AZombie_PlayerController();
	
protected:
	
	virtual void BeginPlay() override;
	
};

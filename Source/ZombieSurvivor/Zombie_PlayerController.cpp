// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie_PlayerController.h"

#include "EnhancedInputSubsystems.h"

AZombie_PlayerController::AZombie_PlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}

void AZombie_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	bShowMouseCursor = true;
	
	FInputModeGameAndUI InputMode;
	
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	
	InputMode.SetHideCursorDuringCapture(false);
	
	SetInputMode(InputMode);
	
}

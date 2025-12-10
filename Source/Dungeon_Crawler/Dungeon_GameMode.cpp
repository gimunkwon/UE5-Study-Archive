// Fill out your copyright notice in the Description page of Project Settings.


#include "Dungeon_GameMode.h"

#include "Dungeon_Character.h"
#include "Dungeon_Controller.h"

#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"


ADungeon_GameMode::ADungeon_GameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("'/Game/BluePrints/BP_Dungeon_Character.BP_Dungeon_Character_C'"));
	
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	
	
	PlayerControllerClass = ADungeon_Controller::StaticClass();
}

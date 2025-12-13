// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie_AIController.h"

#include <Kismet/GameplayStatics.h>
#include <GameFramework/Character.h>

void AZombie_AIController::BeginPlay()
{
	Super::BeginPlay();
	
	
	PrimaryActorTick.bCanEverTick = true;
}

void AZombie_AIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	
	if (PlayerPawn)
	{
	
	
		MoveToActor(PlayerPawn, 200.0f);


	}

}

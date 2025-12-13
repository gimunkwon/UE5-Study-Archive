// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie_GameMode.h"
#include "Zombie_Player.h"
#include "Zombie_PlayerController.h"
#include "Zombie_Enemy.h"

#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"


AZombie_GameMode::AZombie_GameMode()
{
	
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("'/Game/BluePrints/BP_Player.BP_Player_C'"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	
	PlayerControllerClass = AZombie_PlayerController::StaticClass();

}

void AZombie_GameMode::BeginPlay()
{
	Super::BeginPlay();
	
	// 2초마다 SpawnZombie 함수를 무한 반복실행
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AZombie_GameMode::SpawnZombie, SpawnInterval, true);
}

void AZombie_GameMode::SpawnZombie()
{
	// 1. 스폰할 좀비 클래스가 지정되어 있는지 확인
	if (!ZombieClass) return;
	
	// 2. 플레이어 위치 찾기(주인공 주변에 소환해야 하니까)
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn) return;
	
	// 3. 내비게이션 시스템 가져오기
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (NavSystem)
	{
		FNavLocation RandomLocation;
		
		// 4. 플레이어 주변 500cm ~ 2000cm 사이의 랜덤한 길 위 좌표 찾기
		// GetRandomPointInNavigableRadius : 벽속이나 허공이 아닌 갈수있는 땅만 골라줌
		bool bFonud = NavSystem->GetRandomPointInNavigableRadius(PlayerPawn->GetActorLocation(),2000.0f, RandomLocation);
		
		if (bFonud)
		{
			// 5. 좀비 소환
			// z 값을 살짝 올려서 땅에 파묻히지 않게함
			FVector SpawnLoc = RandomLocation.Location + FVector(0, 0 , 50.0f);
			FRotator SpawnRot = FRotator::ZeroRotator;
			
			GetWorld()->SpawnActor<AZombie_Enemy>(ZombieClass, SpawnLoc, SpawnRot);
			
		}
		
		
	}
	
}

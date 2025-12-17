
#include "Soul_Enemy_AIController.h"

#include "BaseEnemy.h"
#include "Kismet/GameplayStatics.h"


ASoul_Enemy_AIController::ASoul_Enemy_AIController()
{
	
	PrimaryActorTick.bCanEverTick = true;
}


void ASoul_Enemy_AIController::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASoul_Enemy_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}


void ASoul_Enemy_AIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// 1. 플레이어 찾기
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	ABaseEnemy* ControlledEnemy = Cast<ABaseEnemy>(GetPawn());
	// 2. 플레이어가 있고 내가 조종하는 폰도 있다면
	if (PlayerPawn && ControlledEnemy)
	{
		// 거리 계산
		float Distance = FVector::Dist(ControlledEnemy->GetActorLocation(), PlayerPawn->GetActorLocation());
		
		// 공격 사거리 안에 들어왔는가
		if (Distance < 150.f)
		{
			ControlledEnemy->Attack();
		}
		else if (Distance < ControlledEnemy->DetectRange)
		{
			// 공격 중이 아닐때만 추격
			if (!ControlledEnemy->IsAttacking())
			{
				// 3. 플레이어 쪽으로 이동
				MoveToActor(PlayerPawn, 100.f);
			}
		}
		else
		{
			StopMovement();
		}
	}
}


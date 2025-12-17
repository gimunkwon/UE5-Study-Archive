
#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "Soul_Enemy_AIController.generated.h"

UCLASS()
class SOULRPG_API ASoul_Enemy_AIController : public AAIController
{
	GENERATED_BODY()

public:
	ASoul_Enemy_AIController();
protected:
	virtual void BeginPlay() override;
	// 빙의(Possess) 되었을 때 실행(게임 시작 시 적이 이 컨트롤러를 장착할때)
	virtual void OnPossess(APawn* InPawn) override;
public:
	virtual void Tick(float DeltaTime) override;
};

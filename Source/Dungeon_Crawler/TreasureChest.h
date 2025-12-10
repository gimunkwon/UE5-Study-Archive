// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionInterface.h"
#include "TreasureChest.generated.h"

UCLASS()
class DUNGEON_CRAWLER_API ATreasureChest : public AActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	ATreasureChest();

protected:
	virtual void BeginPlay() override;
	// 컴포넌트
	// 상자 몸통(루트)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Chest")
	UStaticMeshComponent* BaseMesh;
	// 상자 뚜껑(회전할 부분)
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category="Chest")
	UStaticMeshComponent* LidMesh;
	
	// 변수
	// 이미 열렸는지 체크
	bool bIsOpened = false;
	
	// 이 상자에서 나올 아이템들 (BP_Item_Sword등)
	UPROPERTY(EditAnywhere, Category="Rewards")
	TArray<TSubclassOf<AActor>> ItemClassToSpawn;

public:
	virtual void Tick(float DeltaTime) override;
	
	// 인터페이스 구현
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
protected:
	// C++에서 호출하고, 구현은 블루프린트(Timeline)에서 하는 함수
	// (애니메이션 같은 시각적 연출은 블루프린트가 훨신 편하기 때문)
	UFUNCTION(BlueprintImplementableEvent)
	void OnChestOpened();
};

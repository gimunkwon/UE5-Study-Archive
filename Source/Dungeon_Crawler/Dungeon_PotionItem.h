// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Dungeon_ItemStruct.h"
#include "InteractionInterface.h"
#include "Dungeon_PotionItem.generated.h"

class URotatingMovementComponent;
class USphereComponent;

UCLASS()
class DUNGEON_CRAWLER_API ADungeon_PotionItem : public AActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	ADungeon_PotionItem();
	
	// 인터페이스 함수 구현(Implementation을 붙여야함)
	virtual  void Interact_Implementation(APawn* InstigatorPawn) override;

protected:
	virtual void BeginPlay() override;
	
	// 충돌 감지 영역
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereComp;
	// 포션 모양
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComp;
	// 회복량
	UPROPERTY(EditAnywhere, Category="Item")
	float HealAmount = 30.0f;
	//빙글빙글 돌게 해주는 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement")
	URotatingMovementComponent* RotatingComp;
	// 이 아이템의 정보 (에디터에서의 이름, 아이콘 설정용)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item Data")
	FItemData ItemInfo;


public:
	virtual void Tick(float DeltaTime) override;
};

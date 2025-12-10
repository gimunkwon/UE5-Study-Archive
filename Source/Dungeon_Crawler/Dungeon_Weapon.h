// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Dungeon_ItemStruct.h"
#include "Dungeon_Weapon.generated.h"

class UBoxComponent;

UCLASS()
class DUNGEON_CRAWLER_API ADungeon_Weapon : public AActor
{
	GENERATED_BODY()

public:
	ADungeon_Weapon();
	
	// 외부(캐릭터/노티파이)에서 부를 함수들
	void EnableCollision(); // 판정켜기
	void DisableCollision(); // 판정 끄기
	// 이 무기의 원본 아이템 데이터 (다시 인벤토리로 돌아 갈때 씀)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
	FItemData ItemInfo;
protected:
	virtual void BeginPlay() override;
	
	// 무기 외형(검)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon")
	UStaticMeshComponent* WeaponMesh;
	
	// 공격 판전용 박스 (나중에 공격할때)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon")
	UBoxComponent* CollisionBox;
	
	// 충돌 감지 함수
	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp
		, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
public:
	virtual void Tick(float DeltaTime) override;
};

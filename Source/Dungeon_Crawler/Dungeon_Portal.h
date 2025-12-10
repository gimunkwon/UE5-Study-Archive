// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Dungeon_Portal.generated.h"

class UBoxComponent;

UCLASS()
class DUNGEON_CRAWLER_API ADungeon_Portal : public AActor
{
	GENERATED_BODY()

public:
	
	ADungeon_Portal();

protected:
	virtual void BeginPlay() override;
	
	// 1. 포탈 외형(문틀)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Portal")
	UStaticMeshComponent* PortalMesh;
	// 2. 포탈 이펙트 (보라색 소용돌이 등)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Portal")
	UParticleSystemComponent* PortalEffect;
	// 3. 감지박스(여기에 닿으면 이동)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Portal")
	UBoxComponent* TransferVolume;
	// 4. 이동할 맵 이름 (에디터에서 "DungeonMap"이라고 적어줄 것임)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Portal")
	FName TransferLevelName;
	
	// 닿았을 때 실행할 함수
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp
		, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
public:
	
	virtual void Tick(float DeltaTime) override;
};

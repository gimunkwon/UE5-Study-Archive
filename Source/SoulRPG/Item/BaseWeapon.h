

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

class UBoxComponent;

UCLASS()
class SOULRPG_API ABaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	ABaseWeapon();
	
	// 1. 공격 판정 함수
	void EnableCollision(); // 공격 시작시 호출
	void DisableCollision(); // 공격 끝날때 호출

protected:
	virtual void BeginPlay() override;
	// 무기 외형
	UPROPERTY(VisibleAnywhere, Category="Weapon Properties")
	UStaticMeshComponent* ItemMesh;
	// 나중에 쓸 충돌 판정용 박스
	UPROPERTY(VisibleAnywhere, Category="Weapon Properties")
	UBoxComponent* CollisionBox;
	// 이벤트 함수
	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp
		, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
private:
	// 한 번 휘두를 대 같은적을 여러번 때리는 걸 방지
	TArray<AActor*> IgnoreActors;

public:
	virtual void Tick(float DeltaTime) override;
};

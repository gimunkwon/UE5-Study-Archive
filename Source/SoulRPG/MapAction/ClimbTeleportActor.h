

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ClimbTeleportActor.generated.h"

class UInputMappingContext;
class UInputAction;
class UArrowComponent;
class UBoxComponent;

UCLASS()
class SOULRPG_API AClimbTeleportActor : public AActor
{
	GENERATED_BODY()
public:
	AClimbTeleportActor();
protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
	
	//컴포넌트
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category="Components")
	USceneComponent* RootScene; // 루트
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category="Components")
	UBoxComponent* TriggerBox; // 감지영역
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category="Components")
	UArrowComponent* DestPoint; // 도착지점
	
	// 설정 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Teleport")
	float FadeDuration = 0.5f; // 깜빡이는 시간
	// 입력 관련
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* InteractAction;
	UPROPERTY(EditAnywhere, Category="Input")
	UInputMappingContext* DefaultContext;
	
protected:
	// 내부 함수
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp
		, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResul);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	// E키 눌렀을때 실행
	void OnInteract();
	// 페이드 아웃 후 실제 이동 담당
	void PerformTeleport();
private:
	// 타이머용 핸들
	FTimerHandle TimerHandle_Teleport;
	// 이동할 플레이어 저장용
	class ACharacter* TargetrCharacter;
	
	
};

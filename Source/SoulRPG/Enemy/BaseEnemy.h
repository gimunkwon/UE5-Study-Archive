

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"

class UWidgetComponent;
class UBoxComponent;

UCLASS()
class SOULRPG_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseEnemy();
protected:
	virtual void BeginPlay() override;
	// 체력 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	float MaxHealth = 100.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat")
	float CurrentHealth;
	// 맞았을 때 재생할 몽타주
	UPROPERTY(EditDefaultsOnly, Category="Combat")
	UAnimMontage* HitMontage;
	// 사망처리 함수
	virtual void Die();
	UPROPERTY(EditDefaultsOnly, Category="Combat")
	UAnimMontage* AttackMontage;
	bool bIsAttacking = false;
	// 공격 애니메이션 끝났을 때 불릴 함수
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	// 손에 붙일 충돌 박스
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat")
	UBoxComponent* RightHandCollision;
	// 충돌 감지 함수
	UFUNCTION()
	void OnHandOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp
		, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResul);
	// 체력바 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="UI")
	UWidgetComponent* HealthBarWidget;
public:
	virtual void Tick(float DeltaTime) override;
	// 데미지 함수
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent
		, class AController* EventInstigator, AActor* DamageCauser) override;
	// AI 컨트롤러가 부를 공격 함수
	void Attack();
	// 공격 중인지 확인하는 함수
	bool IsAttacking() const {return bIsAttacking;}
	// 노티파이에서 부를 함수(충돌 켜기/ 끄기)
	UFUNCTION(BlueprintCallable)
	void SetHandCollisionEnabled(bool bEnabled);
	// 감지 범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Category="AI")
	float DetectRange = 600.f;
private:
	// 피격 모션 재생 함수
	void PlayHitMontage(const FName& SectionName);
	
};

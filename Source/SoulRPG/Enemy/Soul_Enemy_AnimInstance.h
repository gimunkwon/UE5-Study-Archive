// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Soul_Enemy_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SOULRPG_API USoul_Enemy_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeInitializeAnimation() override;
protected:
	UPROPERTY(BlueprintReadOnly, Category="Movement")
	class ABaseEnemy* EnemyCharacter;
	// 이동속도
	UPROPERTY(BlueprintReadOnly, Category="Movement")
	float GroundSpeed;
};

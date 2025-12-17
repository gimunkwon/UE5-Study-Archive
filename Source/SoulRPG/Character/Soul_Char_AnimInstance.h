// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Soul_Char_AnimInstance.generated.h"

class UCharacterMovementComponent;
class ASoul_Character;
/**
 * 
 */
UCLASS()
class SOULRPG_API USoul_Char_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	// 초기화 함수
	virtual void NativeInitializeAnimation() override;
	// 업데이트 함수
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	// ----------변수-------------
#pragma region Instance
	// 캐릭터 정보 캐싱
	UPROPERTY(BlueprintReadOnly, Category="Character")
	ASoul_Character* Soul_Character;
	UPROPERTY(BlueprintReadOnly, Category="Character")
	UCharacterMovementComponent* SoulCharacterMovement;
#pragma endregion
	// 캐릭터의 속력값
	UPROPERTY(BlueprintReadOnly, Category="Movement")
	float GroundSpeed;
	// 공중인가? 체크용
	UPROPERTY(BlueprintReadOnly, Category="Movement")
	bool bIsFalling;
};

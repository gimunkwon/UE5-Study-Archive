// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_EnableAction.generated.h"

/**
 * 
 */
UCLASS()
class SOULRPG_API UAN_EnableAction : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	// 몽타주 지나갈 때 딱 한번 실행되는 함수
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};

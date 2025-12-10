// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_AttackWindow.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEON_CRAWLER_API UANS_AttackWindow : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	// 구간 시작될 때 실행
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation
		, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation
		, const FAnimNotifyEventReference& EventReference) override;
	
};

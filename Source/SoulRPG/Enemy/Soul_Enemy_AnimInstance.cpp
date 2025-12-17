// Fill out your copyright notice in the Description page of Project Settings.


#include "Soul_Enemy_AnimInstance.h"

#include "BaseEnemy.h"

void USoul_Enemy_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (EnemyCharacter == nullptr)
	{
		EnemyCharacter = Cast<ABaseEnemy>(TryGetPawnOwner());		
	}
	if (EnemyCharacter)
	{
		FVector Velocity = EnemyCharacter->GetVelocity();
		Velocity.Z = 0;
		GroundSpeed = Velocity.Size();
	}
}

void USoul_Enemy_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	EnemyCharacter = Cast<ABaseEnemy>(TryGetPawnOwner());
}

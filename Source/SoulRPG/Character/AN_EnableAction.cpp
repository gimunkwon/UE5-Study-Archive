// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_EnableAction.h"

#include "Soul_Character.h"

void UAN_EnableAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	
	if (MeshComp && MeshComp->GetOwner())
	{
		// 플레이어 캐릭터 가져오기
		ASoul_Character* Player = Cast<ASoul_Character>(MeshComp->GetOwner());
		if (Player)
		{
			Player->SetState(ECharacterState::Idle);
		}
	}
}

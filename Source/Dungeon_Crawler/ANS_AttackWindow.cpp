// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_AttackWindow.h"

#include "Dungeon_Character.h"
#include "Dungeon_Weapon.h"


void UANS_AttackWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	
	
	if (MeshComp && MeshComp->GetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("공격 판정 ON!!!!"));
		if (ADungeon_Character* Player = Cast<ADungeon_Character>(MeshComp->GetOwner()))
		{
			Player->SetWeaponCollision(true);
		}
	}
}

void UANS_AttackWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	if (MeshComp && MeshComp->GetOwner())
	{
		if (ADungeon_Character* Player = Cast<ADungeon_Character>(MeshComp->GetOwner()))
		{
			Player->SetWeaponCollision(false);
		}
	}
}

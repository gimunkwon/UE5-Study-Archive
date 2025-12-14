// Fill out your copyright notice in the Description page of Project Settings.


#include "Soul_Char_AnimInstance.h"

#include "Soul_Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void USoul_Char_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	// 1. 소유자(Pawn)을 가져와서 내 캐릭터인지 확인하고 캐싱
	Soul_Character = Cast<ASoul_Character>(TryGetPawnOwner());
	if (Soul_Character)
	{
		SoulCharacterMovement = Soul_Character->GetCharacterMovement();
	}
}

void USoul_Char_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (SoulCharacterMovement == nullptr)
	{
		Soul_Character = Cast<ASoul_Character>(TryGetPawnOwner());
		if (Soul_Character)
		{
			SoulCharacterMovement = Soul_Character->GetCharacterMovement();
		}
		return;
	}
	
	GroundSpeed = UKismetMathLibrary::VSizeXY(SoulCharacterMovement->Velocity);
	bIsFalling = SoulCharacterMovement->IsFalling();
}

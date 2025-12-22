


#include "Soul_Character_HUD.h"

#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "SoulRPG/Character/Soul_Character.h"

void USoul_Character_HUD::SetHealth(float CurrentHealth, float MaxHealth)
{
	if (HealthBar)
	{
		float Percent = CurrentHealth / MaxHealth;
		HealthBar->SetPercent(Percent);
	}
}

void USoul_Character_HUD::OnInventoryClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("인벤토리 클릭!!"));
	// 1. 현재 이 위젯을 보고있는 플레이어 캐릭터 가져옴
	APawn* Pawn = GetOwningPlayerPawn();
	
	ASoul_Character* MyChar = Cast<ASoul_Character>(Pawn);
	
	if (MyChar)
	{
		UE_LOG(LogTemp, Warning, TEXT("인벤토리 함수 호출됨"));
		MyChar->ToggleInvnentory();
	}
}

void USoul_Character_HUD::OnEquipmentClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("장비창 클릭!!"));
}

void USoul_Character_HUD::NativeConstruct()
{
	Super::NativeConstruct();
	if (EquipmentButton)
	{
		EquipmentButton->OnClicked.AddDynamic(this, &USoul_Character_HUD::OnEquipmentClicked);
	}
	if (InventoryButton)
	{
		InventoryButton->OnClicked.AddDynamic(this, &USoul_Character_HUD::OnInventoryClicked);
	}
	
}




#include "Soul_Character_HUD.h"

#include "Components/ProgressBar.h"

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
	
}

void USoul_Character_HUD::OnEquipmentClicked()
{
	
}

void USoul_Character_HUD::NativeConstruct()
{
	Super::NativeConstruct();
}

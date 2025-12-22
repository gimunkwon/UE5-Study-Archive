

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Soul_Character_HUD.generated.h"


class UButton;

UCLASS()
class SOULRPG_API USoul_Character_HUD : public UUserWidget
{
	GENERATED_BODY()
public:
	// 체력 업데이트하는 함수
	void SetHealth(float CurrentHealth, float MaxHealth);
	
	
protected:
	UPROPERTY(meta=(BindWidget))
	class UProgressBar* HealthBar;
	UPROPERTY(meta=(BindWidgetOptional))
	UButton* InventoryButton;
	UPROPERTY(meta=(BindWidgetOptional))
	UButton* EquipmentButton;
	
	// 버튼 클릭시 호출될 함수
	UFUNCTION()
	void OnInventoryClicked();
	UFUNCTION()
	void OnEquipmentClicked();
	
	virtual void NativeConstruct() override;
};

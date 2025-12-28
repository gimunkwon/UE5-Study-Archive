#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SoulRPG/Character/Component/Soul_InventoryComponent.h"
#include "InventorySlotWidget.generated.h"

class UButton;
// 델리게이트 선언
// 파라미터로 FItemData를 넘겨줌
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnslotClicked, const FItemData&, ItemData);

class UTextBlock;
class UImage;
struct FItemData;


UCLASS()
class SOULRPG_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	// 델리게이트 변수
	// 메인 위젯이 여기에 반응
	UPROPERTY(BlueprintAssignable, Category="Event")
	FOnslotClicked OnItemClicked;
	
	
	// 외부에서 데이터를 넣어주면 UI 를 갱신하는 함수
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void SetItemData(const FItemData& Data);
	void SetIsEmpty();
	
protected:
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	UFUNCTION(BlueprintImplementableEvent, Category="Event")
	void OnRightClicked();
	
	UPROPERTY(meta=(BindWidgetOptional))
	UImage* Image_Icon;
	UPROPERTY(meta=(BindWidgetOptional))
	UTextBlock* Text_Count;
	UPROPERTY(meta=(BindWidgetOptional))
	UButton* SlotButton;
	// 버튼 클릭시 실행할 함수
	UFUNCTION()
	void OnButtonClicked();
	UFUNCTION(BlueprintCallable)
	FItemData GetMyItemData() const{return MyItemData;}
	
private:
	FItemData MyItemData;
	
};

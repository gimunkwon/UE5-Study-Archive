// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Soul_InventoryWidget.generated.h"

class UInventorySlotWidget;
class UWrapBox;
struct FItemData;
class UButton;

UCLASS()
class SOULRPG_API USoul_InventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// 인벤토리 데이터를 받아서 목록을 싹 새로고침하는 함수
	UFUNCTION(BlueprintCallable)
	void RefreshInventory(const TArray<FItemData>& Items);
	// 슬롯이 클릭됏을때 실행할 함수
	UFUNCTION()
	void HandleSlotClicked(const FItemData& ItemData);
	UFUNCTION(BlueprintCallable, Category="UI")
	UWrapBox* GetItemListWrapBox() const {return ItemListWrapBox;}
	
protected:
	virtual void NativeOnInitialized() override;
	UPROPERTY(meta=(BindWidget),BlueprintReadWrite)
	UWrapBox* ItemListWrapBox;
	// 생성할 슬롯 위젯 클래스
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UInventorySlotWidget> SlotWidgetClass;	
	
	UPROPERTY(meta=(BindWidget))
	UButton* CloseButton;
	UFUNCTION()
	void OnCloseButtonClicked();
	
	// 우측 상세 정보창 위젯들 바인딩
	UPROPERTY(meta=(BindWidget))
	class UImage* Image_DetailIcon;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* Text_DetailName;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* Text_DetailDesc;
	// 우측 상세 정보창 전체를 감싸는 박스
	UPROPERTY(meta=(BindWidget))
	class UVerticalBox* VBox_ItemDetail;
	
	
};

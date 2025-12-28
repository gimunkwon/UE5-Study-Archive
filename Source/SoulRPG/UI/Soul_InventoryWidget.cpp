// Fill out your copyright notice in the Description page of Project Settings.


#include "Soul_InventoryWidget.h"
#include "InventorySlotWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/WrapBox.h"
#include "SoulRPG/Character/Soul_Character.h"

#include "SoulRPG/Character/Component/Soul_InventoryComponent.h"

void USoul_InventoryWidget::RefreshInventory(const TArray<FItemData>& Items)
{
	if (!ItemListWrapBox || !SlotWidgetClass) return;
	
	// 1. 기존 목록 비우기(초기화)
	ItemListWrapBox->ClearChildren();
	
	// 2. 아이템 개수만큼 반복문 돌려서 슬롯 생성
	for (const FItemData& Item : Items)
	{
		// 슬롯 위젯 생성
		UInventorySlotWidget* NewSlot = CreateWidget<UInventorySlotWidget>(this, SlotWidgetClass);
		
		if (NewSlot)
		{
			// 데이터 넣어주기
			NewSlot->SetItemData(Item);
			// 클릭되면 함수 실행
			NewSlot->OnItemClicked.AddDynamic(this, &USoul_InventoryWidget::HandleSlotClicked);
			ItemListWrapBox->AddChildToWrapBox(NewSlot);
		}
	}
	if (VBox_ItemDetail)
	{
		// Hidden: 안 보이지만 자리는 차지함
		VBox_ItemDetail->SetVisibility(ESlateVisibility::Hidden);
	}
	
	// 남는 공간을 빈 슬롯으로 채우기(최소25칸 유지)
	int32 MinSlots = 25;
	int32 CurrentSlots = Items.Num();
	int32 EmptySlotsNeeded = MinSlots - CurrentSlots;
	
	if (EmptySlotsNeeded > 0)
	{
		for (int32 i = 0; i < EmptySlotsNeeded; ++i)
		{
			// 빈 슬롯 위젯을 생성
			UInventorySlotWidget* EmptySlot = CreateWidget<UInventorySlotWidget>(this, SlotWidgetClass);
			if (EmptySlot)
			{
				// 빈 슬롯임을 알리는 함수 호출
				EmptySlot->SetIsEmpty();
				ItemListWrapBox->AddChildToWrapBox(EmptySlot);
			}
		}
	}
}

void USoul_InventoryWidget::HandleSlotClicked(const FItemData& ItemData)
{
	// 이름 변경
	if (Text_DetailName)
	{
		Text_DetailName->SetText(FText::FromName(ItemData.ItemName));
	}
	// 설명 변경
	if (Text_DetailDesc)
	{
		Text_DetailDesc->SetText(ItemData.ItemDescription);
	}
	// 이미지 변경
	if (Image_DetailIcon && ItemData.ItemIcon)
	{
		Image_DetailIcon->SetBrushFromTexture(ItemData.ItemIcon);
		Image_DetailIcon->SetVisibility(ESlateVisibility::Visible);
	}
	// 상세 정보창 보여주기
	if (VBox_ItemDetail)
	{
		VBox_ItemDetail->SetVisibility(ESlateVisibility::Visible);
	}
}

void USoul_InventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (CloseButton)
	{
		CloseButton->OnClicked.AddDynamic(this, &USoul_InventoryWidget::OnCloseButtonClicked);
	}
}


void USoul_InventoryWidget::OnCloseButtonClicked()
{
	ASoul_Character* OwningChar = Cast<ASoul_Character>(GetOwningPlayerPawn());
	if (OwningChar)
	{
		OwningChar->ToggleInvnentory();
	}
}



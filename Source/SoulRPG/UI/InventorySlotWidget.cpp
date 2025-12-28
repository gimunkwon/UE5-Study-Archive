


#include "InventorySlotWidget.h"

#include "Soul_InventoryWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "SoulRPG/Character/Component/Soul_InventoryComponent.h"

void UInventorySlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	// 버튼이 클릭되면 OnButtonClicked실행해라
	if (SlotButton)
	{
		SlotButton->OnClicked.AddDynamic(this, &UInventorySlotWidget::OnButtonClicked);
	}
}

FReply UInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// 우클릭인지 확인
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		// 아이템이 있는 경우에만 메뉴 오픈
		if (!MyItemData.ItemID.IsNone())
		{
			// 블루프린트 이벤트 호출
			OnRightClicked();
			
			return FReply::Handled();
		}
	}
	
	
	
	
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UInventorySlotWidget::SetItemData(const FItemData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("아이템 [%s] 수량 : %d"),*Data.ItemName.ToString(),Data.Amount);
	// 데이터를 받으면 변수에 저장
	MyItemData = Data;
	
	// 아이콘 설정
	if (Image_Icon && Data.ItemIcon)
	{
		Image_Icon->SetBrushFromTexture(Data.ItemIcon);
		Image_Icon->SetColorAndOpacity(FLinearColor(1.f,1.f,1.f,1.f));
	}
	if (Text_Count)
	{
		Text_Count->SetText(FText::AsNumber(Data.Amount));
		Text_Count->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UInventorySlotWidget::OnButtonClicked()
{
	// 버튼이 눌리면 방송
	if (OnItemClicked.IsBound())
	{
		OnItemClicked.Broadcast(MyItemData);
	}
}

void UInventorySlotWidget::SetIsEmpty()
{
	// 버튼은 비활성화하거나 투명하게
	if (SlotButton) SlotButton->SetIsEnabled(false);
	if (Image_Icon) Image_Icon->SetVisibility(ESlateVisibility::Hidden); 
}

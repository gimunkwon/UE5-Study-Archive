


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

void UInventorySlotWidget::SetItemData(const FItemData& Data)
{
	// 데이터를 받으면 변수에 저장
	MyItemData = Data;
	
	// 아이콘 설정
	if (Image_Icon && Data.ItemIcon)
	{
		Image_Icon->SetBrushFromTexture(Data.ItemIcon);
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

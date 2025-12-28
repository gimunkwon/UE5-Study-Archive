
#include "Soul_InventoryComponent.h"

USoul_InventoryComponent::USoul_InventoryComponent()
{
	
	
	PrimaryComponentTick.bCanEverTick = true;

	
}

void USoul_InventoryComponent::SetQuickSlot(int32 SlotIndex, FName ItemID)
{
	// 데이터 저장
	QuickSlots.Add(SlotIndex, ItemID);
	// 아이템 데이터 찾기
	FItemData* FoundItem = GetItemData(ItemID);
	
	if (FoundItem)
	{
		// 방송 송출
		OnQuickSlotUpdated.Broadcast(SlotIndex, *FoundItem);
		UE_LOG(LogTemp, Warning, TEXT("퀵슬롯 [%d] 업데이트 방송 송출 완료"),SlotIndex);
	}
	
	
}

FName USoul_InventoryComponent::UseItemFromQuickSlot(int32 SlotIndex)
{
	// 해당 퀵슬롯에 등록된 아이템 ID가 있는지 확인
	if (!QuickSlots.Contains(SlotIndex))
	{
		return FName(TEXT("None")); // 등록된 게 없음
	}
	
	FName TargetItemID = QuickSlots[SlotIndex];
	// 인벤토리 배열을 뒤져서 실제 아이템 찾기
	for (int32 i = 0; i < InventoryItems.Num(); i++)
	{
		if (InventoryItems[i].ItemID == TargetItemID)
		{
			// 찾음 수량 확인
			if (InventoryItems[i].Amount > 0)
			{
				// 수량 감소
				InventoryItems[i].Amount--;
				UE_LOG(LogTemp, Warning, TEXT("퀵슬롯 [%d] 아이템 사용: %s (낭은 수량: %d)"),
					SlotIndex, *TargetItemID.ToString(), InventoryItems[i].Amount);
				
				// 데이터 백업 UI 갱신용으로 현재 상태 복사
				FItemData UpdatedItem = InventoryItems[i];
				
				// 수량이 0이 되면 목록에서 제거
				if (InventoryItems[i].Amount <= 0)
				{
					InventoryItems.RemoveAt(i);
					// 삭제된 경우 UI에는 빈 껍데기 정보를 보내줘야함
					UpdatedItem.Amount = 0;
				}
				// 인벤토리 UI 갱신
				if (OnInventoryUpdated.IsBound()) OnInventoryUpdated.Broadcast();
				// 퀵슬롯 UI 갱신
				if (OnQuickSlotUpdated.IsBound()) OnQuickSlotUpdated.Broadcast(SlotIndex, UpdatedItem);
				
				// 사용한 아이템 ID 리턴
				return TargetItemID;
			}
		}
	}
	
	// 인벤토리에 아이템이 없으면
	UE_LOG(LogTemp, Warning, TEXT("퀵슬롯에 등록된 아이템이 인벤토리에 없습니다."));
	return FName(TEXT("None"));
}

FItemData* USoul_InventoryComponent::GetItemData(FName ItemID)
{
	// Content는 인벤토리 아이템들이 들어있는 배열이라고 가정
	for (FItemData& Item : InventoryItems)
	{
		if (Item.ItemID == ItemID)
		{
			return &Item;
		}
	}
	
	return nullptr;
}

void USoul_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// 테스트 코드

	// for (int i = 0; i < 36; ++i)
	// {
	// 	GiveItem("HP_Potion");
	// 	if (i == 35)
	// 	{
	// 		UE_LOG(LogTemp, Warning, TEXT("HP포션 지급완료"));
	// 	}
	// }
	GiveItem("HP_Potion");
	GiveItem("HP_Potion");
}

void USoul_InventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USoul_InventoryComponent::GiveItem(FName ItemID)
{
	// 데이터 테이블이 연결 되있는지 확인
	if (!ItemDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("데이터 테이블이 설정안됨"));
		return;
	}
	
	// 데이터 테이블에서 Row Name(ItemID)으로 정보 찾기
	static const FString ContextString(TEXT("Item Lookup")); // 에러 로그용 태그
	FItemData* ItemRow = ItemDataTable->FindRow<FItemData>(ItemID, ContextString);
	
	if (ItemRow)
	{
		// 찾았으면 인벤토리 배열에 추가
		AddToInventory(*ItemRow);
		
		UE_LOG(LogTemp, Warning, TEXT("아이템 흭득 성공: %s"), *ItemRow->ItemName.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("아이템을 찾을수 없습니다 ID를 확인하세요: %s"), *ItemID.ToString());
	}
}

void USoul_InventoryComponent::AddToInventory(FItemData NewItem)
{
	if (NewItem.Amount <= 0)
	{
		NewItem.Amount = 1;
	}
	// 겹칠수 있는 아이템인지 확인
	if (NewItem.bIsStackable)
	{
		// 인벤토리 배열을 순회하며 같은 아이템 찾기
		// 복사본이 아니라 원본을 수정해야하기에 참조사용
		
		for (FItemData& ExistingItem : InventoryItems)
		{
			if (ExistingItem.ItemID == NewItem.ItemID)
			{
				ExistingItem.Amount += NewItem.Amount;
				UE_LOG(LogTemp, Warning, TEXT("[%s] 중복 흭득! 수량증가 -> 현재 :%d개")
					,*ExistingItem.ItemName.ToString(), ExistingItem.Amount);
				// UI 갱신 방송 후 함수 종료
				if (OnInventoryUpdated.IsBound()) OnInventoryUpdated.Broadcast();
				return;
			}
		}
	}
	 //겹치는 아이템이 아니거나 신규아이템이면
	 InventoryItems.Add(NewItem);
	 UE_LOG(LogTemp, Warning, TEXT("[%s] 신규 흭득!"),*NewItem.ItemName.ToString());
	 if (OnInventoryUpdated.IsBound()) OnInventoryUpdated.Broadcast();
}




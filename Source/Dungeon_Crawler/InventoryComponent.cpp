
#include "InventoryComponent.h"
#include "Dungeon_Character.h"


UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;


}

void UInventoryComponent::AddItem(FItemData NewItem)
{
	// 가방이 꽉 찼으면 못 넣음
	if (Items.Num() >= Capacity)
	{
		UE_LOG(LogTemp, Warning, TEXT("인벤토리가 가득 찼습니다."));
		return;
	}
	
	// 배열에 추가
	Items.Add(NewItem);
	
	// 로그 출력
	UE_LOG(LogTemp, Warning, TEXT("아이템 흭득! : %s"), *NewItem.ItemName.ToString());
	// UI에게 갱신 방송
	OnInventoryUpdated.Broadcast();
}


void UInventoryComponent::UseItem(int32 Index)
{
	// 유효한 인덱스인지 확인
	if (Items.IsValidIndex(Index))
	{
		FItemData& Item = Items[Index];
		
		// 1. 클릭 감지 확인
		UE_LOG(LogTemp, Warning, TEXT("아이템 사용 시도: %s (타입 : %d)"), *Item.ItemName.ToString(),(int32)Item.ItemType);
		
		// 내 주인 가져오기
		ADungeon_Character* Character = Cast<ADungeon_Character>(GetOwner());
		if (!Character) return;
		
		bool bUsed = false;
		// 아이템 타입별 동작
		if (Item.ItemType == EItemType::Potion)
		{
			// 캐릭터의 Heal 함수 호출
			bUsed = Character->Heal(Item.ItemValue);
		}
		else if (Item.ItemType == EItemType::Weapon)
		{
			// ItemClass가 무기인지 확인하고 변환
			if (Item.ItemClass)
			{
				TSubclassOf<ADungeon_Weapon> WeaponClass = *Item.ItemClass;
				Character->EquipWeaponFromInventory(WeaponClass);
				
				UE_LOG(LogTemp, Warning, TEXT("무기 교체함"));
				bUsed = true;
			}
			
		}
		
		// 아이템을 사용했다면 가방에서 삭제
		if (bUsed)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s 사용 완료!"), *Item.ItemName.ToString());
			Items.RemoveAt(Index); // 배열에서 제거
			
			// UI 갱신 방송
			OnInventoryUpdated.Broadcast();
		}
	}
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	
	
}


void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}


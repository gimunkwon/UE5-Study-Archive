
#include "Soul_InventoryComponent.h"

USoul_InventoryComponent::USoul_InventoryComponent()
{
	
	
	PrimaryComponentTick.bCanEverTick = true;

	
}

void USoul_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// 테스트 코드

	for (int i = 0; i < 36; ++i)
	{
		GiveItem("Weapon_OldSword");
	}
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
		InventoryItems.Add(*ItemRow);
		
		UE_LOG(LogTemp, Warning, TEXT("아이템 흭득 성공: %s"), *ItemRow->ItemName.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("아이템을 찾을수 없습니다 ID를 확인하세요: %s"), *ItemID.ToString());
	}
}




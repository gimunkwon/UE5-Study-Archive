

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Soul_InventoryComponent.generated.h"

// 아이템 타입 구분용 (무기, 방어구, 소모품)
UENUM(BlueprintType)
enum class EItemType : uint8
{
	Weapon		UMETA(DisplayName = "Weapon"),
	Armor		UMETA(DisplayName = "Armor"),
	Consumabler	UMETA(DisplayName = "Consumable"),
	Etc			UMETA(DisplayName = "Etc")
};

// 아이템 데이터를 담을 구조체 (이후 데이터 테이블과 연동)
USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()
public:
	// 아이템 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;
	// 아이콘 이미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ItemIcon;
	// 아이템 ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;
	// 아이템 설명
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MultiLine = "true"))
	FText ItemDescription;
	// 아이템 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType ItemType;
	// 스탯(value)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ItemValue;
	// 겹치기 가능 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsStackable;
	// 현재 수량 (기본값 1)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Amount;
	// 구조체 생성자 (기본값을 1로 설정) 이걸 안하면 처음생길때 0생기는거 방지
	FItemData()
		: ItemValue(0.0f),Amount(1),bIsStackable(true)
	{}
};

// 델리게이트(구조체 FItemData를 넘겨줌)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);


// 몇번 슬롯이 어떤 아이템으로 변했는지 방송
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FonQuickSlotUpdated, int32, SlotIndex, const FItemData&, ItemData);



UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SOULRPG_API USoul_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	USoul_InventoryComponent();
	// 실제 아이템들이 담길 배열
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory")
	TArray<FItemData> InventoryItems;
	// 에디터에서 만든 DT_ItemData를 넣을 변수
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="Data")
	class UDataTable* ItemDataTable;
	// 퀵 슬롯용 데이터 저장(슬롯번호 : 아이템 ID)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	TMap<int32, FName> QuickSlots;
	// 블루 프린터에서 호출할 함수 (슬롯번호 0, 1, 2, 3, / 아이템 ID)
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void SetQuickSlot(int32 SlotIndex, FName ItemID);
	// 퀵슬롯 번호로 아이템 사용
	UFUNCTION(BlueprintCallable, Category="Inventory")
	FName UseItemFromQuickSlot(int32 SlotIndex);
	
	// 델리게이트 변수 생성
	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FonQuickSlotUpdated OnQuickSlotUpdated;
	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FOnInventoryUpdated OnInventoryUpdated;
	// 아이템 ID로 전체 데이터를 찾는 헬퍼 함수
	FItemData* GetItemData(FName ItemID);
	
protected:
	virtual void BeginPlay() override;
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	// 아이템 추가
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void GiveItem(FName ItemID);
	// 아이템 중첩 함수
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void AddToInventory(FItemData NewItem);
};

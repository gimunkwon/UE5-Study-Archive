

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
};

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
protected:
	virtual void BeginPlay() override;
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	// 아이템 추가
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void GiveItem(FName ItemID);
};

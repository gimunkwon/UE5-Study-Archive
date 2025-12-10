#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Dungeon_ItemStruct.generated.h"

// 아이템 종류(무기 포션등)
UENUM(BlueprintType)
enum class EItemType : uint8
{
	Weapon UMETA(DisplayName = "Weapon"),
	Potion UMETA(DisplayName = "Potion"),
	Etc UMETA(DisplayName = "Etc"),
};

// 아이템 하나에 대한 정보 구조체
// FTableRowBase를 상속받으면 나중에 엑셀(DataTable)로 관리가능
USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	// 아이템 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
	FText ItemName;
	// 아이템 아이콘 (UI에 표시될 이미지)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
	UTexture2D* ItemIcon;
	// 아이템 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
	EItemType ItemType;
	// 회복량이나 공격력 같은 수치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
	float ItemValue;
	// 아이템 사용 시 소환될 액터클래스 (무기 BP등)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
	TSubclassOf<AActor> ItemClass;
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Dungeon_ItemStruct.h"
#include "InventoryComponent.generated.h"

// 델리게이트: 인벤토리가 변했을때 UI에게 화면갱신해 라고 알려주는 신호
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DUNGEON_CRAWLER_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();

	// 아이템 추가 함수
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void AddItem(FItemData NewItem);
	
	// 인벤토리 내용물 (배열)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory")
	TArray<FItemData> Items;
	// 인벤토리 최대 칸수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
	int32 Capacity = 20;
	//UI 갱신용 델리게이트
	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FOnInventoryUpdated OnInventoryUpdated;
	// 인덱스(몇 번째 칸)를 받아서 아이템 사용
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void UseItem(int32 Index);
	
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};

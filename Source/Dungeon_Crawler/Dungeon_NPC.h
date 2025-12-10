// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionInterface.h"
#include "Dungeon_DialogueWidget.h"
#include "Dungeon_ItemStruct.h"
#include "GameFramework/Character.h"
#include "Dungeon_NPC.generated.h"

UCLASS()
class DUNGEON_CRAWLER_API ADungeon_NPC : public ACharacter, public IInteractionInterface
{
	GENERATED_BODY()

public:
	ADungeon_NPC();
protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
protected:
	// 위젯 설계도 (WBP_Dialogue를 넣을 곳)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UDungeon_DialogueWidget> DialogueWidgetClass;
	// 현재 떠 있는 위젯 인스턴스
	UPROPERTY()
	UDungeon_DialogueWidget* DialogueInstance;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Quest Reward")
	FItemData QuestRewardItem;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "QuestComponent.h"
#include "Dungeon_Character.generated.h"

#pragma region Forward Declarations
class UInventoryComponent;
class UDungeon_StatWidget;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class USpringArmComponent;
class UCameraComponent;
class UAnimMontage;
class ADungeon_Weapon;
#pragma endregion Forward Declarations
UCLASS()
class DUNGEON_CRAWLER_API ADungeon_Character : public ACharacter
{
	GENERATED_BODY()

public:
	ADungeon_Character();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
	virtual void BeginPlay() override;
#pragma region Category(Camera)
	// 카메라 컴포넌트
	UPROPERTY(VisibleAnywhere, Category="Camera")
	USpringArmComponent* SpringArmComponent;
	UPROPERTY(VisibleAnywhere, Category="Camera")
	UCameraComponent* CameraComponent;
#pragma endregion Category(Camera)
#pragma region Category(Combat)
	// 에디터 에서 "어떤 무기(BP_Sword)를 줄지 고르는 칸"
	UPROPERTY(EditDefaultsOnly, Category="Combat")
	TSubclassOf<ADungeon_Weapon> DefaultWeaponClass;
	
	// 실제로 생성된 무기를 잡아두는 포인터
	UPROPERTY(EditDefaultsOnly, Category="Combat")
	ADungeon_Weapon* EquippedWeapon;
	
	// 공격 몽타주 (에디터에서 AM_Attack 넣을 곳)
	UPROPERTY(EditDefaultsOnly, Category="Combat")
	UAnimMontage* AttackMontage;
#pragma endregion Category(Combat)
	// 스탯
#pragma region Category(Stats)
	UPROPERTY(EditAnywhere, Category="Stats")
	float MaxHealth = 100.0f;
	float CurrentHealth;
	
	UPROPERTY(EditAnywhere, Category="Stats")
	float MaxMana = 100.0f;
	float CurrentMana;
	
	// UI
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UDungeon_StatWidget> HUDClass;
	
	UPROPERTY()
	UDungeon_StatWidget* ActiveHUD;
#pragma endregion Category(Stats)
	//인벤토리
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory")
	UInventoryComponent* InventoryComp;
	//UI 변수
	UPROPERTY(EditDefaultsOnly, Category= "UI")
	TSubclassOf<UUserWidget> InventoryUIClass;
	UPROPERTY()
	UUserWidget* InventoryWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UQuestComponent* QuestComp;
	// 키 바인딩용 함수
	void ToggleInventory();
	
public:
#pragma region InputMapping
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputMappingContext* DefaultMappingContext;
	
	// IA_Move 함수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* EquipAction; // 장착키
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* AttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* InventoryAction;
#pragma endregion InputMapping
	// 노티파이가 부를 함수
	void SetWeaponCollision(bool bEnable);
	// 데미지 받는 함수
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent
		, class AController* EventInstigator, AActor* DamageCauser) override;
	// 회복함수
	bool Heal(float Amount);
	// 외부에서 내 인벤토리에 접근할수있게 해주는 함수(Getter)
	UInventoryComponent* GetInventory() const{return InventoryComp;}
	// 새로운 무기 클래스를 받아서 장착하는 함수
	void EquipWeaponFromInventory(TSubclassOf<class ADungeon_Weapon> NewWeaponClass);
	// 컴포넌트 가져오는 함수 (적이나 NPC가 쓸 수 있게)
	UQuestComponent* GetQuestComponent() const{return QuestComp;}
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* InteractAction;
	
protected:
	void Move(const FInputActionValue& Value);
	
	// 무기를 들고 있는가? (True: 손, False: 등)
	bool bIsWeaponEquipped = false;
	
	// 장착/해제 함수
	void ToggleWeapon();
	
	// 공격 함수
	void Attack();
	
	// 상호작용 시도 함수
	void Interact();

};

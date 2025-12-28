
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Soul_Character.generated.h"

#pragma region Forward Declarations
class USoul_InventoryWidget;
class USoul_InventoryComponent;
class USoul_Character_HUD;
class ABaseWeapon;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;
#pragma endregion

// 캐릭터의 상태 정의
UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Idle	UMETA(DisplayName = "Idle"), // 대기(이동 가능)
	Rolling	UMETA(DisplayName = "Rolling"), // 구르기 (이동 불가)
	Attacking	UMETA(DisplayName = "Attacking"), // 공격 (이동 불가)
	Dead		UMETA(DisplayName = "Dead"), // 사망 (조작 불가)
};


UCLASS()
class SOULRPG_API ASoul_Character : public ACharacter
{
	GENERATED_BODY()
public:
	ASoul_Character();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:
	virtual void BeginPlay() override;
#pragma region CameraComponent
	// 카메라 지지대
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camara")
	USpringArmComponent* CameraBoom;
	// 실제 카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camara")
	UCameraComponent* FollowCamera;
#pragma endregion
	// 구르기 추진력
	UPROPERTY(EditAnywhere, Category="Combat")
	float RollImpulse = 600.f;
	// 현재 상태
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State")
	ECharacterState CurrentState = ECharacterState::Idle;
#pragma region Attack
	// 콤보 몽타주 파일
	UPROPERTY(EditAnywhere, Category="Combat")
	UAnimMontage* AttackMontage;
	// 현재 콤보 카운트(0= 안함, 1 = 1타, 2 = 2타)
	int32 ComboCount = 0;
	// 최대 콤보 횟수
	int32 MaxComboCount = 3;
	// 공격 함수
	void Attack();
	// 선입력(예약) 되었는가?
	bool bHasQueuedInput;
	// 애니메이션 노티파이에서 호출할 검문소 함수
	UFUNCTION(BlueprintCallable)
	void ComboAction();
#pragma endregion
	// 공격이 끝났을 때(몽타주 종료) 초기화할 함수
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
public:
#pragma region InputActions
	// 입력 에셋
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* RollAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* AttackAction;
	// 퀵슬롯 입력 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* QuickSlot1Aciton;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* QuickSlot2Aciton;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* QuickSlot3Aciton;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* QuickSlot4Aciton;
#pragma endregion
	
#pragma region AnimMontage
	// 구르기 몽타주
	UPROPERTY(EditDefaultsOnly, Category="Combat")
	UAnimMontage* RollMontage;
#pragma endregion 
	
#pragma region MovementValue
	// 걷는 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	float WalkSpeed = 500.f;
	// 달리는 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	float RunSpeed = 800.f;
#pragma endregion
	
#pragma region AnimNotify
	// ----헬퍼 함수----
	// 상태 확인용 헬퍼 함수
	bool IsBusy() const; // 구르거나 공격 중인가? (행동 불가 상태)
	// 상태 변경 함수 (Setter)
	void SetState(ECharacterState NewState);
	// 애니메이션 노티파이에서 부를 함수
	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(bool bEnabled);
#pragma endregion
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent
		, class AController* EventInstigator, AActor* DamageCauser) override;
	
#pragma region CameraEffects
	// 구르기 시작할 때 호출(줌 인)
	UFUNCTION(BlueprintImplementableEvent, Category="Camera")
	void StartRollZoom();
	UFUNCTION(BlueprintImplementableEvent, Category="Camera")
	void StopRollZoom();
#pragma endregion 
	
#pragma region InventoryComponent
	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	TSubclassOf<UUserWidget> InventoryClass;
	UPROPERTY()
	USoul_InventoryWidget* InventoryWidget;
	// 인벤토리 함수
	void ToggleInvnentory();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory")
	USoul_InventoryComponent* InventoryComp;
#pragma endregion
	
protected:
#pragma region InputFunc
	// 이동
	void Move(const FInputActionValue& Value);
	// 시선(마우스)
	void Look(const FInputActionValue& Value);
	// 달리기
	void Sprint();
	// 달리기 멈춤
	void StopSprint();
	// 구르기 함수
	void Roll();
	// 마우스 커서 방향으로 캐릭터 회전 시키기
	void RotateToMouseCursor();
	// 퀵슬롯 입력 처리 함수
	void OnQuickSlot1();
	void OnQuickSlot2();
	void OnQuickSlot3();
	void OnQuickSlot4();
#pragma endregion
	
#pragma region Weapon
	// 에디터에서 지정할 기본 무기 클래스
	UPROPERTY(EditDefaultsOnly, Category="Combat")
	TSubclassOf<ABaseWeapon> DefaultWeaponClass;
	// 실제 장착된 무기 포인터
	UPROPERTY(VisibleInstanceOnly)
	ABaseWeapon* EquippedWeapon;
	// 무기 장착 함수
	void EquipWeapon(ABaseWeapon* WeaponToEquip);
#pragma endregion

#pragma region HPComponent
	// 체력 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Category="Status")
	float MaxHealth = 100.f;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category="Status")
	float CurrentHealth;
	// 위젯 클래스
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UUserWidget> HUDClass;
	// 위젯 변수
	UPROPERTY()
	USoul_Character_HUD* MainHUD;
#pragma endregion
	
	// 포션 함수
	void ApplyItemEffect(FName ItemID);
	
};

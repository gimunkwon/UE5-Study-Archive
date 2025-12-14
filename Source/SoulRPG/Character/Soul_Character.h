
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Soul_Character.generated.h"
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;

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
protected:
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:
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
#pragma endregion
	// 구르기 몽타주
	UPROPERTY(EditDefaultsOnly, Category="Combat")
	UAnimMontage* RollMontage;
	// 걷는 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	float WalkSpeed = 500.f;
	// 달리는 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	float RunSpeed = 800.f;
	// ----헬퍼 함수----
	// 상태 확인용 헬퍼 함수
	bool IsBusy() const; // 구르거나 공격 중인가? (행동 불가 상태)
	// 상태 변경 함수 (Setter)
	void SetState(ECharacterState NewState);
	// 구르기 시작할 때 호출(줌 인)
	UFUNCTION(BlueprintImplementableEvent, Category="Camera")
	void StartRollZoom();
	UFUNCTION(BlueprintImplementableEvent, Category="Camera")
	void StopRollZoom();
protected:
#pragma region InputFunc
	// 이동
	void Move(const FInputActionValue& Value);
	// 시선(마우스)
	void Look(const FInputActionValue& Value);
#pragma endregion
	// 달리기
	void Sprint();
	// 달리기 멈춤
	void StopSprint();
	// 구르기 함수
	void Roll();
};

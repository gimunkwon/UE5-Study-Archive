


#include "Soul_Character.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "SoulRPG/Item/BaseWeapon.h"


ASoul_Character::ASoul_Character()
{
 	
	PrimaryActorTick.bCanEverTick = true;
	
#pragma region CharacterMovementComponent
	// 1. 컨트롤러 회전 분리 (마우스 돌려도 캐릭터는 안 돔)
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	// 2. 이동 방향으로 캐릭터 회전 (가려는 방향을 바라봄)
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // 회전속도
	// 점프 높이 등등
	GetCharacterMovement()->JumpZVelocity = 700.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
#pragma endregion
#pragma region CameraComponent
	// 3. 카메라 붐(지지대)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f; // 캐릭터와의 거리
	CameraBoom->bUsePawnControlRotation = true; // 마우스(컨트롤러) 회전을 따라감
	// 4. 카메라
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; // 카메라는 붐만 따라가면 됨
#pragma endregion
	
}

void ASoul_Character::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem 
			= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	if (DefaultWeaponClass)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			ABaseWeapon* SpawnedWeapon =  World->SpawnActor<ABaseWeapon>(DefaultWeaponClass);
			if (SpawnedWeapon)
			{
				EquipWeapon(SpawnedWeapon);
			}
		}
	}
	CurrentHealth = MaxHealth;
}

void ASoul_Character::Attack()
{
	// 1. 구르는 중이거나 죽었으면 공격 불가
	if (CurrentState == ECharacterState::Rolling || CurrentState == ECharacterState::Dead) return;
	
	// 2. 변경점 이미 공격중이라면 -> 예약만 하고 끝냄
	if (CurrentState == ECharacterState::Attacking)
	{
		bHasQueuedInput = true;
		UE_LOG(LogTemp, Warning, TEXT("다음 공격 예약됨!"));
		return;
	}
	// 3. 공격 중이 아니라면(처음클릭)-> 바로 실행 로직 호출
	ComboAction();
}

void ASoul_Character::ComboAction()
{
	// A. 처음 때리는 거면 (Idle)
	if (CurrentState == ECharacterState::Idle)
	{
		ComboCount = 1;
	}
	// B. 연타 치는거
	else if (bHasQueuedInput)
	{
		bHasQueuedInput = false;
		ComboCount++; // 다음 콤보로
		if (ComboCount > MaxComboCount) ComboCount = 1;
	}
	// C. 공격 중인데도 예약이 없다(타이밍 놓침)
	else
	{
		return;
	}
	
	// 2. 몽타주가 없으면 실행 불가
	if (AttackMontage == nullptr) return;
	// 4. 재생할 섹션 이름 만들기 (Attack_1, Attack_2....)
	
	if (AttackMontage)
	{
		FName SectionName = FName(*FString::Printf(TEXT("Attack_%d"), ComboCount));
		
		PlayAnimMontage(AttackMontage, 0.8f, SectionName);
		
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			FOnMontageEnded EndDelegate;
			EndDelegate.BindUObject(this, &ASoul_Character::OnAttackMontageEnded);
			GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(EndDelegate, AttackMontage);
		}
	}
	// 6. 상태변경
	SetState(ECharacterState::Attacking);
}

void ASoul_Character::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (bInterrupted)
	{
		return;
	}
	
	
	// 공격이 끝나면 상태 초기화 & 콤보 초기화
	if (Montage == AttackMontage)
	{
		SetState(ECharacterState::Idle);
		ComboCount = 0; // 공격 끈힉면 0으로 리셋
		bHasQueuedInput = false;
		UE_LOG(LogTemp, Warning, TEXT("공격 종료! 콤보 리셋"))
	}
}

void ASoul_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GEngine)
	{
		float CurrentSpeed = GetCharacterMovement()->MaxWalkSpeed;
		GEngine->AddOnScreenDebugMessage(1, 0.0f, FColor::Red
			, FString::Printf(TEXT("현재 플레이어 속도: %f"),CurrentSpeed));
	}
}

void ASoul_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASoul_Character::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASoul_Character::Look);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ASoul_Character::Sprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ASoul_Character::StopSprint);
		EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Started, this, &ASoul_Character::Roll);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ASoul_Character::Attack);
	}
}

bool ASoul_Character::IsBusy() const
{
	// 구르거나 공격 중이면 바쁨
	return CurrentState == ECharacterState::Rolling || CurrentState == ECharacterState::Rolling 
	|| CurrentState == ECharacterState::Dead;
}

void ASoul_Character::SetState(ECharacterState NewState)
{
	CurrentState = NewState;
}

void ASoul_Character::SetWeaponCollisionEnabled(bool bEnabled)
{
	if (EquippedWeapon)
	{
		if (bEnabled)
		{
			EquippedWeapon->EnableCollision();
			UE_LOG(LogTemp, Warning, TEXT("무기 충돌 켜짐"))
		}
		else
		{
			EquippedWeapon->DisableCollision();
			UE_LOG(LogTemp, Warning, TEXT("무기 충돌 꺼짐"))
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("오류: EquippedWeapon이 비어있음"));
	}
}

float ASoul_Character::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	CurrentHealth = FMath::Clamp(CurrentHealth - ActualDamage, 0.f, MaxHealth);
	
	UE_LOG(LogTemp, Error, TEXT("플레이어 피격 남은 체력:%f"),CurrentHealth);
	if (CurrentHealth <= 0.f)
	{
		UE_LOG(LogTemp, Error, TEXT("플레이어 사망"));
	}
	
	
	return ActualDamage;
}

void ASoul_Character::Move(const FInputActionValue& Value)
{
	if (IsBusy()) return;
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		// 1. 컨트롤러(카메라)가 보는 방향 알아내기
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0); // 높낮이(Pitch)는 무시하고 수평(Yaw)만
		
		// 2. 그 방향의 앞과 오른쪽 벡터 구하기
		const FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		// 3. 실제 이동 시키기
		AddMovementInput(ForwardVector, MovementVector.Y); // W, S
		AddMovementInput(RightDirection, MovementVector.X); // A, D
	}
}

void ASoul_Character::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		// 마우스 입력대로 카메라 돌리기
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ASoul_Character::Sprint()
{
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void ASoul_Character::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ASoul_Character::Roll()
{
	// 이미 바쁘면 구르기 불가
	if (IsBusy()) return;
	
	// 1. 이미 몽타주가 재생 중이면 무시(캔슬 방지)
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AnimInstance->IsAnyMontagePlaying())
	{
		return;
	}
	// 2. 몽타주 재생
	if (RollMontage)
	{
		PlayAnimMontage(RollMontage);
		// 구르는동안 무적판정 or 스태미나 깎는 로직
		// 상태 변경
		SetState(ECharacterState::Rolling);
		
		// 구르기 시작 줌 땡기기
		StartRollZoom();
		
		// 몽타주 종료 시 상태 복구 예약
		if (AnimInstance)
		{
			FOnMontageEnded EndDelegate;
			EndDelegate.BindWeakLambda(this, [this](UAnimMontage* Montage, bool bInterrupted)
			{
				if (Montage == RollMontage)
				{
					// 상태 복구: 다시 대기 상태로
					SetState(ECharacterState::Idle);
					// 구르기 끝 줌 풀기
					StopRollZoom();
					UE_LOG(LogTemp, Warning, TEXT("구르기종료 -> Idle"))
				}
			});
			AnimInstance->Montage_SetEndDelegate(EndDelegate, RollMontage);
		}
	}
	// 강제 이동 로직(LaunchCharacter)
	// 이동 하려는 방향(Input)이 있으면 그쪽으로, 없으면 보는 방향(Forward)으로 구른다.
	FVector RollDirection = GetLastMovementInputVector();
	if (RollDirection.IsNearlyZero())
	{
		RollDirection = GetActorForwardVector();
	}
	else
	{
		// 입력한 방향이 있다면, 몸을 즉시 그쪽으로 돌려라
		FRotator TargetRotation = RollDirection.Rotation();
		SetActorRotation(TargetRotation);
	}
	// LaunchCharacter(힘,XY덮어쓰기, Z 덮어쓰기)
	// true, false: 기존 속도 무시하고(true) 밀어버리되, 중력(Z)는 유지(false)해라.
	
	FVector LaunchVelocity = RollDirection * RollImpulse;
	LaunchCharacter(LaunchVelocity, true, false);
	UE_LOG(LogTemp, Warning, TEXT("현재 구르기 속도: %f"),RollImpulse);
	UE_LOG(LogTemp, Warning, TEXT("구르기 발동"));
}

void ASoul_Character::EquipWeapon(ABaseWeapon* WeaponToEquip)
{
	if (WeaponToEquip == nullptr) return;
	
	const FName SocketName = FName("WeaponSocket");
	// FAttachmentTransformRules::SnapToTarget : 위치와 회전을 소켓에 딱 맞춤
	WeaponToEquip->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
	
	// 무기의 주인을 나로 설정(데미지 계산시 필요)
	WeaponToEquip->SetOwner(this);
	WeaponToEquip->SetInstigator(this);
	
	EquippedWeapon = WeaponToEquip;
}



#include "Dungeon_Character.h"
#include "Dungeon_Weapon.h"
#include "Dungeon_StatWidget.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InteractionInterface.h"
#include "InventoryComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"


ADungeon_Character::ADungeon_Character()
{
	PrimaryActorTick.bCanEverTick = true;
	
#pragma region SpringArm&&Camera_Settings
	
	// 1. 스프링 암
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	
	SpringArmComponent->TargetArmLength = 600.f; 
	SpringArmComponent->SetRelativeRotation(FRotator(-50.0f, 0.0f, 0.0f)); // 50도 각도
	SpringArmComponent->bUsePawnControlRotation = false;
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->bInheritPitch = false;
	SpringArmComponent->bInheritRoll = false;
	SpringArmComponent->bInheritYaw = false;
	
	// 카메라 랙(Lag) 켜기
	// 캐릭터가 움직일 때 카메라가 약간 늦게 따라오면서 부드러운 느낌을 줌
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->CameraLagSpeed = 10.0f;
	
	// 2. 카메라
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;
	
	// 3. 회전 설정 (WASD 이동 방향 보게 하기)
	
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // 좀더 빠른 회전
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	
#pragma endregion
	
	// 인벤토리 생성
	InventoryComp = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComp"));
	QuestComp = CreateDefaultSubobject<UQuestComponent>(TEXT("QuestComp"));
	
}

void ADungeon_Character::BeginPlay()
{
	Super::BeginPlay();
	
	// 1. 스탯 초기화
	CurrentHealth = MaxHealth;
	CurrentMana = MaxMana;
	
	UE_LOG(LogTemp,Warning,TEXT("플레이어의 현재 체력: %f"),CurrentHealth);
	
	// 2. HUD 생성 및 부착
	if (HUDClass)
	{
		if (APlayerController* PC = Cast<APlayerController>(Controller))
		{
			ActiveHUD = CreateWidget<UDungeon_StatWidget>(PC, HUDClass);
			if (ActiveHUD)
			{
				ActiveHUD->AddToViewport();
				ActiveHUD->UpdateHealth(CurrentHealth, MaxHealth);
				ActiveHUD->UpdateMana(CurrentMana, MaxMana);
			}
		}
	}
	
	
	// 플레이어 컨트롤러 맵핑
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// 무기 클래스가 지정되있다면
	if (DefaultWeaponClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("무기 클래스 지정 완료!"));
		// 1. 무기 소환 (Spawn)
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this; // 내거다
		SpawnParams.Instigator = GetInstigator();
		
		// 월드에 무기 생성
		EquippedWeapon = GetWorld()->SpawnActor<ADungeon_Weapon>(DefaultWeaponClass, GetActorTransform(), SpawnParams);
		
		// 2. 등 뒤에 붙이기 (Attach)
		if (EquippedWeapon)
		{
		
			// "WeaponBackSocket"이라는 이름의 소켓에 붙여라
			// SnapToTarget: 위치와 회전을 소켓에 딱 맞춤
			FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
			EquippedWeapon->AttachToComponent(GetMesh(), AttachRules, TEXT("WeaponBackSocket"));
		}
		
	}
	
}

void ADungeon_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADungeon_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADungeon_Character::Move);
		
		// 장착 키 바인딩 (Started: 누르는 순간 딱 한번)
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Started, this, &ADungeon_Character::ToggleWeapon);
		// 공격 바인딩
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ADungeon_Character::Attack);
		// 인벤토리 바인딩
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &ADungeon_Character::ToggleInventory);
		// 상호작용 바인딩
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ADungeon_Character::Interact);
	}

}

void ADungeon_Character::ToggleInventory()
{
	if (!InventoryUIClass) return;
	
	if (!InventoryWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("인벤토리 활성화"));
		// 처음 누르면 생성
		InventoryWidget = CreateWidget<UUserWidget>(GetWorld(), InventoryUIClass);
	}
	if (InventoryWidget)
	{
		if (InventoryWidget->IsInViewport())
		{
			// 이미 켜져있으면 끈다
			InventoryWidget->RemoveFromParent();
			
			// 입력모드 : 게임으로 복귀
			if (APlayerController* PC = Cast<APlayerController>(Controller))
			{
				PC->SetInputMode(FInputModeGameAndUI());
				PC->bShowMouseCursor = true;
			}
		}
		else
		{
			// 꺼져있으면 켠다
			InventoryWidget->AddToViewport();
		}
	}
	
}

void ADungeon_Character::SetWeaponCollision(bool bEnable)
{
	if (EquippedWeapon)
	{
		if (bEnable)
		{
			EquippedWeapon->EnableCollision();
		}
		else
		{
			EquippedWeapon->DisableCollision();
		}
	}
}

float ADungeon_Character::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	// 체력 감소
	CurrentHealth = FMath::Clamp(CurrentHealth - ActualDamage, 0.0f, MaxHealth);
	
	// HUD 갱신
	if (ActiveHUD)
	{
		ActiveHUD->UpdateHealth(CurrentHealth, MaxHealth);
	}
	
	if (CurrentHealth <= 0.0f)
	{
		// 사망처리
	}
	
	return ActualDamage;
}

bool ADungeon_Character::Heal(float Amount)
{
	// 이미 풀피면 안 먹음
	if (CurrentHealth >= MaxHealth) return false;
	
	// 체력 증가
	CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0.0f, MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("회복! 현재 체력 : %f"), CurrentHealth);
	
	// UI 갱신
	if (ActiveHUD)
	{
		ActiveHUD->UpdateHealth(CurrentHealth, MaxHealth);
	}
	
	return true;
}

void ADungeon_Character::EquipWeaponFromInventory(TSubclassOf<class ADungeon_Weapon> NewWeaponClass)
{
	if (!NewWeaponClass) return;
	
	// 1. 기존에 들고 있던 무기 인벤토리로 반환 (Swap)
	if (EquippedWeapon)
	{
		// 인벤토리 컴포넌트 가져오기
		if (UInventoryComponent* Inv = GetInventory())
		{
			// 기존 무기의 정보를 가방에 넣음
			Inv->AddItem(EquippedWeapon->ItemInfo);
			
			UE_LOG(LogTemp, Warning, TEXT("기존 무기 인벤토리로 반환됨!"));
		}
		
		// 이제 파괴
		EquippedWeapon->Destroy();
	}
	// 2. 새 무기 소환
	FActorSpawnParameters SpawnParms;
	SpawnParms.Owner = this;
	SpawnParms.Instigator = GetInstigator();
	
	EquippedWeapon = GetWorld()->SpawnActor<ADungeon_Weapon>(NewWeaponClass, GetActorTransform(), SpawnParms);
	
	// 3. 손에 장착
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponHandSocket"));
		bIsWeaponEquipped = true;
		
		UE_LOG(LogTemp, Warning, TEXT("새 무기 장착 완료!"));
	}
}

void ADungeon_Character::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2d>();
	
	if (Controller != nullptr)
	{
		const FVector ForwardDirection = FVector::ForwardVector;
		const FVector RightDirection = FVector::RightVector;
		
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ADungeon_Character::ToggleWeapon()
{
	// 무기가 없으면 아무것도 안 함
	if (!EquippedWeapon) return;
	
	// 소켓 이름 정의(오타 방지용)
	const FName BackSocket = TEXT("WeaponBackSocket");
	const FName HandSocket = TEXT("WeaponHandSocket");
	
	if (bIsWeaponEquipped)
	{
		// [상태 : 손 -> 등] 집어넣기
		EquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, BackSocket);
		bIsWeaponEquipped = false;
		UE_LOG(LogTemp, Warning, TEXT("무기 집어넣음"));
		
	}
	else
	{
		// [상태 : 등 -> 손] (꺼내기)
		EquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, HandSocket);
		bIsWeaponEquipped = true;
		UE_LOG(LogTemp, Warning, TEXT("무기 장착함"));
	}
	
}

void ADungeon_Character::Attack()
{
	// 1. 무기를 안 들고 있으면(등에 있으면) 공격 불가
	if (!bIsWeaponEquipped) return;
	
	// 2. 몽타주가 있으면 재생
	if (AttackMontage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			// 이미 몽타주가 재생 중이면 다시 재생하지 않음
			if (!AnimInstance->Montage_IsPlaying(AttackMontage))
			{
				AnimInstance->Montage_Play(AttackMontage);
			}
		}
	}
}

void ADungeon_Character::Interact()
{
	// 1. 시선방향으로 레이저 쏘기
	FVector Start = GetActorLocation();
	FVector End = Start + (GetActorForwardVector() * 150.0f); // 전방 2m
	
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this); // 나는무시
	
	FCollisionShape Shape = FCollisionShape::MakeSphere(50.0f);
	
	// Visibility 채널로 검사
	bool bHit = GetWorld()->
	SweepSingleByChannel(HitResult, Start, End, FQuat::Identity,ECC_Visibility, Shape, Params);
	// 디버깅용 스윕 모양 그리기
	DrawDebugSphere(GetWorld(), Start, 50.0f, 12, FColor::Green, false, 1.0f);
	DrawDebugSphere(GetWorld(), End, 50.0f, 12, FColor::Green, false, 1.0f);
	
	if (bHit)
	{
		AActor* HitActor = HitResult.GetActor();
		
		// 2. 맞은 놈이 인터페이스를 가지고 있는지 확인
		if (HitActor && HitActor->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
		{
			// 3. 상호작용 실행
			// Execute_함수명(대상, 인자...) 형태로 호출
			IInteractionInterface::Execute_Interact(HitActor, this);
		}
	}
	
	
	
}

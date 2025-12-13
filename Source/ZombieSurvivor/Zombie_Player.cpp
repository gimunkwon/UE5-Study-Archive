
#include "Zombie_Player.h"
#include "Zombie_Projectile.h"
#include  "Zombie_HUD.h"

#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>

#include <GameFramework/CharacterMovementComponent.h>
#include <GameFramework/PlayerController.h>

#include <Kismet/GameplayStatics.h>
#include <Engine/World.h>
#include "Blueprint/UserWidget.h"
#include <Sound/SoundBase.h>
#include "Particles/ParticleSystem.h"

#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>

#include "Components/CapsuleComponent.h"


AZombie_Player::AZombie_Player()
{
	PrimaryActorTick.bCanEverTick = true;
	
#pragma region SpringArm&&Camera
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);

	
	SpringArmComponent->TargetArmLength = 1000.0f; 
	SpringArmComponent->SetRelativeRotation(FRotator(-60.0f, 0.0f, 0.0f)); 
	SpringArmComponent->bUsePawnControlRotation = false; 
	SpringArmComponent->bDoCollisionTest = false; 
	SpringArmComponent->bInheritPitch = false;
	SpringArmComponent->bInheritRoll = false;
	SpringArmComponent->bInheritYaw = false;


	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;

	
	bUseControllerRotationYaw = false; 
	GetCharacterMovement()->bOrientRotationToMovement = true; 
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f);
#pragma endregion


}

void AZombie_Player::BeginPlay()
{
	Super::BeginPlay();
	
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (SpawnVoiceSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, SpawnVoiceSound, GetActorLocation());
	}
	
	
	// 1. 체력 초기화
	CurrentHealth = MaxHealth;
	// 1. 탄약 초기화
	CurrentAmmo = MaxAmmo;
	// 2. HUD 생성 및 부착
	if (HUDClass)
	{
		ActiveHUD = CreateWidget<UZombie_HUD>(GetWorld(), HUDClass);
		if (ActiveHUD)
		{
			ActiveHUD->AddToViewport(); // 화면에 띄우기
			ActiveHUD->UpdateHealth(CurrentHealth, MaxHealth); // 처음엔 꽉 채워서 보여주기
			ActiveHUD->UpdateAmmo(CurrentAmmo, MaxAmmo);
		}
		
	}
	
	
}

void AZombie_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	RotateToCursor();
}

void AZombie_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AZombie_Player::Move);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AZombie_Player::Fire);
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &AZombie_Player::Reload);
	}
}

float AZombie_Player::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	// 1. 체력 감소 (0보다 작아지지 않게 Clamp)
	CurrentHealth = FMath::Clamp(CurrentHealth - ActualDamage, 0.0f, MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("플레이어 쳐력: %f"), CurrentHealth);
	
	// 2. HUD 갱신 요청
	if (ActiveHUD)
	{
		ActiveHUD->UpdateHealth(CurrentHealth, MaxHealth);
	}
	
	// 3. 사멍 처리 (체력이 0이 되면)
	if (CurrentHealth <= 0.0f)
	{
		// 움직임 멈추고 로그 출력
		// 나중에 여기서 게임 오버 UI출력
		UE_LOG(LogTemp, Error, TEXT("플레이어 사망!"));
		// 입력 끄기
		if (APlayerController* PC = Cast<APlayerController>(Controller))
		{
			DisableInput(PC);
			PC->bShowMouseCursor = false;
		}
		
		// 2. 캡슐 컴포넌트 끄기 (더 이상 충돌 안되게)
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		// 3. 랙돌 활성화 - 시체가 물리엔진으로 털썩
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
		
		// 게임 오버 UI 출력 추가
		if (GameOverWidgetClass)
		{
			// 1. 위젯 생성
			UUserWidget* GameOverWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass);
			if (GameOverWidget)
			{
				GameOverWidget->AddToViewport();
			}
		
			// 2. 마우스 커서 보이게 하기
			if (APlayerController* PC = Cast<APlayerController>(Controller))
			{
				PC->bShowMouseCursor = true;
			
				// 3. 입력 모드를 UI 전용으로 변경
				FInputModeUIOnly InputMode;
				InputMode.SetWidgetToFocus(GameOverWidget->TakeWidget());
				PC->SetInputMode(InputMode);
			}
		}
		
	}
	
	
	
	
	return ActualDamage;
}

void AZombie_Player::Move(const FInputActionValue& Value)
{
	
	FVector2D MovementVector = Value.Get<FVector2D>();

	//UE_LOG(LogTemp, Warning, TEXT("Input X: %f, Y:%f"), MovementVector.X, MovementVector.Y);

	if (Controller != nullptr)
	{
		
		
		const FVector ForwardDirection = FVector::ForwardVector;
		const FVector RightDirection = FVector::RightVector;

		
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AZombie_Player::RotateToCursor()
{
	
	APlayerController* PC = Cast<APlayerController>(Controller);
	if (PC)
	{
		FHitResult HitResult;

		
		bool bHit = PC->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

		if (bHit)
		{
			
			FVector LookAtLocation = HitResult.Location;
			FVector MyLocation = GetActorLocation();

			
			LookAtLocation.Z = MyLocation.Z;


			FRotator LookRotation = (LookAtLocation - MyLocation).Rotation();


			SetActorRotation(LookRotation);



		}
	}
}

void AZombie_Player::Fire(const FInputActionValue& Value)
{
	// 1. 총알이 없으면 발사 불가
	if (CurrentAmmo <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("탄약 부족"));
		return;
	}
	
	// 2. 총알 감소
	CurrentAmmo--;
	
	if (ProjectileClass)
	{
	
		FVector SpawnLocation = GetActorLocation() + (GetActorForwardVector() * 50.0f);
		FRotator SpawnRotation = GetActorRotation();
		
		// 2. 스폰 파라미터 설정
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this; // 이 총알의 주인은 나다
		SpawnParams.Instigator = GetInstigator(); // 이 데미지의 원인 제공자도 나다.
		
		
		GetWorld()->SpawnActor<AZombie_Projectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
		
		// 이펙트 재생 코드 추가
		if(MuzzleEffect)
		{
			// 내 몸통 위치 + 앞쪽으로 조금 떨어진 곳
			FVector MuzzleLocation = GetActorLocation() + (GetActorForwardVector() * 100.0f);
			
			// SpawnEmitterAttached : 캐릭터한테 붙어서 따라다니게 함
			// SpawnEmitterAtLocation: 그 자리에 고정됨
			
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleEffect, MuzzleLocation,GetActorRotation(),FVector(0.5f));
			
			
		}
		
		// 발사 소리 재생
		if (FireSound)
		{
			// 내 위치에서 소리 재생 (볼륨 0.5 피치 1.0)
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation(), 0.5f);
		}

	}
	
	// HUD 갱신
	if (ActiveHUD)
	{
		ActiveHUD->UpdateAmmo(CurrentAmmo, MaxAmmo);
	}
}

void AZombie_Player::Reload(const FInputActionValue& Value)
{
	// 탄약 다시채우기
	if (CurrentAmmo < MaxAmmo)
	{
		CurrentAmmo = MaxAmmo;
	}
	// UI 갱신
	if (ActiveHUD)
	{
		ActiveHUD->UpdateAmmo(CurrentAmmo, MaxAmmo);
	}
}


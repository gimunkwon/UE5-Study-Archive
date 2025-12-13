// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie_Enemy.h"
#include "Zombie_AIController.h"
#include "Zombie_HUD.h"
#include "Zombie_Player.h"

#include "Components/WidgetComponent.h"
#include <GameFramework/CharacterMovementComponent.h>

#include <Components/CapsuleComponent.h>
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AZombie_Enemy::AZombie_Enemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	AIControllerClass = AZombie_AIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned; 
	
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	
	UCapsuleComponent* Capsule = GetCapsuleComponent();
	Capsule->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Capsule->SetGenerateOverlapEvents(true);
	
	// 1. 위젯 컴포넌트 생성
	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidgetComponent"));
	HealthWidgetComponent->SetupAttachment(GetRootComponent());
	
	// 2. 위치 및 설정 잡기
	HealthWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 90.0f)); // 머리 위로 올림
	HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen); // 항상 카메라를 정면으로 바라보게함
	HealthWidgetComponent->SetDrawSize(FVector2D(100.0f, 15.0f)); // 위젯크기
	
	AttackRangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttackRangeSphere"));
	AttackRangeSphere->SetupAttachment(GetRootComponent());
	AttackRangeSphere->SetSphereRadius(60.0f); // 캡슐(몸통)보다 살짝 더크게
	AttackRangeSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic")); // 겹침 전용
	
	// 2. 이벤트 바인딩
	AttackRangeSphere->OnComponentBeginOverlap.AddDynamic(this, &AZombie_Enemy::OnAttackRangeOverlap);
	AttackRangeSphere->OnComponentEndOverlap.AddDynamic(this, &AZombie_Enemy::OnAttackRangeEndlap);
}


float AZombie_Enemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInsigator, AActor* DamageCauser)
{
	
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInsigator, DamageCauser);

	
	if (ActualDamage > 0.0f)
	{
		CurrentHealth -= ActualDamage;
		UE_LOG(LogTemp, Warning, TEXT("좀비체력 : %f"), CurrentHealth);

	
		if (CurrentHealth <= 0.0f)
		{
			UE_LOG(LogTemp, Warning, TEXT("좀비사망!"));
			Destroy();
		}
	}
	
	// 3. 위젯 갱신 로직 추가
	if (HealthWidgetComponent)
	{
		// 위젯 컴포넌트가 가지고 있는 '실제 위젯'을 가져와서 형변환
		UZombie_HUD* EnemyHUD = Cast<UZombie_HUD>(HealthWidgetComponent->GetUserWidgetObject());
		
		if (EnemyHUD)
		{
			EnemyHUD->UpdateHealth(CurrentHealth, MaxHealth);
		}
	}

	return ActualDamage;
}

// Called when the game starts or when spawned
void AZombie_Enemy::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}

// 실제 데미지 주는 함수
void AZombie_Enemy::Attack()
{
	// 내 근처에 있는 플레이어를 찾아서 (타이머가 돌때 플레이어가 죽거나 사라졌을 수도 있으니 확인)
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	
	// 거리 체크 (혹시 버그로 멀리 있는데 때리는거 방지)
	if (PlayerPawn && GetDistanceTo(PlayerPawn) < 200.0f)
	{
		// 10 데미지 주기
		UGameplayStatics::ApplyDamage(PlayerPawn, 10.0f, GetController(), this, UDamageType::StaticClass());
		
	}
	
}

void AZombie_Enemy::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	// 닿은 게 플레이어라면
	if (Cast<AZombie_Player>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("Attack함수 활성화"));
		// A. 닿자마자 한 대 때림
		Attack();
		
		// B. 그리고 0.5초마다 계속 때리라고 타어머 설정 (Loop)
		GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &AZombie_Enemy::Attack, 1.0f, true);
	}
	
}

void AZombie_Enemy::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	
	// 플레이어가 도망갔다면
	if (Cast<AZombie_Player>(OtherActor))
	{
		// 타이머 해제
		GetWorldTimerManager().ClearTimer(AttackTimerHandle);
	}
}

void AZombie_Enemy::OnAttackRangeOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 나 자신 무시 & 플레이어인지 확인
	if (OtherActor == this) return;
	
	if (Cast<AZombie_Player>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("공격 범위 진입! 때리기 시작"));
		
		// 때리기
		Attack();
		
		// 반복 타이머 시작
		GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &AZombie_Enemy::Attack, 1.0f, true);
		
		
	}
}

void AZombie_Enemy::OnAttackRangeEndlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<AZombie_Player>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("플레이어 도망감 공격 중지"));
		
		// 타이머 해제
		GetWorldTimerManager().ClearTimer(AttackTimerHandle);
		
	}
}

// Called every frame
void AZombie_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GEngine)
	{
		
	}
}

// Called to bind functionality to input
void AZombie_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


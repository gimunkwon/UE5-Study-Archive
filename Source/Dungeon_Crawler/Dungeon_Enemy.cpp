// Fill out your copyright notice in the Description page of Project Settings.


#include "Dungeon_Enemy.h"
#include "Dungeon_Character.h"
#include "Dungeon_DamageTextActor.h"
#include "Dungeon_StatWidget.h"

#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADungeon_Enemy::ADungeon_Enemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// 1. 몬스터는 메시가 캡슐 안에 잘 들어가게 조정
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f ,-90.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	// 2. 콜리전 설정(무기랑 부딪히면 Block이 아니라 Overlap 되거나, 무기는 Block하되 카메라는 무시하거나 등등)
	// 일단 기본 Character 설정을 따르기
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	
	// 위젯 컴포넌트 생성
	HealthWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidgetComp"));
	HealthWidgetComp->SetupAttachment(RootComponent);
	HealthWidgetComp->SetRelativeLocation(FVector(0.0f, 0.0f, 90.0f));
	HealthWidgetComp->SetWidgetSpace(EWidgetSpace::Screen); // 빌보드
	HealthWidgetComp->SetDrawSize(FVector2D(100.0f, 15.0f));
}

// Called when the game starts or when spawned
void ADungeon_Enemy::BeginPlay()
{
	Super::BeginPlay();
	
	// 체력 초기화
	CurrentHealth = MaxHealth;
	
}

// Called every frame
void ADungeon_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADungeon_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ADungeon_Enemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	if (CurrentHealth > 0.0f)
	{
		// 체력 감소
		CurrentHealth = FMath::Clamp(CurrentHealth - ActualDamage, 0.0f, MaxHealth);
		UE_LOG(LogTemp, Warning, TEXT("몬스터 피격! 남은 체력: %f"), CurrentHealth);
		
		// 위젯 갱신
		if (HealthWidgetComp)
		{
			UDungeon_StatWidget* EnemyHUD = Cast<UDungeon_StatWidget>(HealthWidgetComp->GetUserWidgetObject());
			if (EnemyHUD)
			{
				EnemyHUD->UpdateHealth(CurrentHealth, MaxHealth);
			}
		}
		
		// 사망 체크
		if (CurrentHealth <= 0.0f)
		{
			// 나를 때린 놈(DamageCauser)이 플레이어 혹은 플레이어의 무기인지 확인
			AActor* KillerActor = DamageCauser;
			
			// 만약 때린게 무기라면 그 무기의 주인인 캐릭터를 가져옴
			if (KillerActor && KillerActor->GetOwner())
			{
				KillerActor = KillerActor->GetOwner();
			}
			// 이제 범인이 플레이어 캐릭터인지 확인
			ADungeon_Character* Player = Cast<ADungeon_Character>(KillerActor);
			if (Player)
			{
				// 퀘스트 컴포넌트를 가져와서 적 죽였다 알림
				if (UQuestComponent* Quest = Player->GetQuestComponent())
				{
					UE_LOG(LogTemp, Error , TEXT("적 사망!!"));
					Quest->OnEnemykilled();
				}
			}
			
			Die();
		}
		
		// 데미지 폰트 소환
		if (DamageTextClass)
		{
			// 몬스터 머리 위쪽에서 소환
			FVector SpawnLocation = GetActorLocation() + FVector(0.0f, 0.0f, 100.0f);
			
			// 랜덤성 추가 (글자가 겹치지 않게 살짝 랜덤 위치)
			SpawnLocation.X += FMath::RandRange(-20.0f, 20.0f);
			SpawnLocation.Y += FMath::RandRange(-20.0f, 20.0f);
			
			FRotator SpawnRotation = FRotator::ZeroRotator;
			
			ADungeon_DamageTextActor* DamageText = GetWorld()->SpawnActor<ADungeon_DamageTextActor>(DamageTextClass, SpawnLocation, SpawnRotation);
			
			if (DamageText)
			{
				DamageText->SetDamage(ActualDamage); // 데미지 숫자 전달
			}
			
		}
		
	}
	
	
	
	return ActualDamage;
}

void ADungeon_Enemy::AttackCheck()
{
	// 내 근처에 있는 플레이어를 찾아서
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	
	// 플레이어가 있고, 거리가 가까우면
	if (PlayerPawn && GetDistanceTo(PlayerPawn) < 200.0f)
	{
		// 10 데미지 주기
		UGameplayStatics::ApplyDamage(PlayerPawn, 10.0f
			, GetController(), this, UDamageType::StaticClass());
		
		UE_LOG(LogTemp, Warning, TEXT("플레이어 타격!"));
	}
	
}

void ADungeon_Enemy::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	// 닿은게 플레이어
	if (Cast<ADungeon_Character>(OtherActor))
	{
		// A. 닿자마자 일단 한 대 때림
		AttackCheck();
		
		// B. 계속 붙어있으면 1초마다 때리라고 타이머 설정
		GetWorldTimerManager().SetTimer(AttackTimerHandle, this
			, &ADungeon_Enemy::AttackCheck, 1.0f, true );
	}
}

void ADungeon_Enemy::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	
	// 플레이어가 도망갔다면
	if (Cast<ADungeon_Character>(OtherActor))
	{
		// 타이머 해제
		GetWorldTimerManager().ClearTimer(AttackTimerHandle);
	}
}

void ADungeon_Enemy::Die()
{
	// 이미 죽었으면 패스
	if (GetMesh()->IsSimulatingPhysics()) return;
	
	UE_LOG(LogTemp, Error, TEXT("몬스터 사망!"));
	
	// 1. 캡슐 끄기
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	// 2. 랙돌 켜기
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
	
	
	// 3. 5초뒤 삭제
	SetLifeSpan(5.0f);
}


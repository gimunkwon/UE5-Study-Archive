


#include "BaseEnemy.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SoulRPG/UI/Soul_Character_HUD.h"


ABaseEnemy::ABaseEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// 박스 컴포넌트 생성
	RightHandCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightHandBox"));
	RightHandCollision->SetupAttachment(GetMesh(), "Character1_RightHand");
	// 평소엔 꺼두기
	RightHandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightHandCollision->SetCollisionResponseToAllChannels(ECR_Overlap);
	RightHandCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Overlap);
	
	// 위젯 컴포넌트 생성및 부착
	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());
	// 2. 위치 및 설정
	HealthBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 120.0f));
	HealthBarWidget->SetWidgetSpace(EWidgetSpace::Screen); // 항상 카메라를 정면으로 바라보게함(빌보드)
	HealthBarWidget->SetDrawSize(FVector2D(150.f, 20.f));
	
}


void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
	RightHandCollision->OnComponentBeginOverlap.AddDynamic(this, &ABaseEnemy::OnHandOverlap);
	
	USoul_Character_HUD* EnemyHUD = Cast<USoul_Character_HUD>(HealthBarWidget->GetUserWidgetObject());
	if (EnemyHUD)
	{
		EnemyHUD->SetHealth(CurrentHealth, MaxHealth);
	}
}


void ABaseEnemy::Die()
{
	// 이미 죽은 상태면 중복 실행 방지
	if (GetMesh()->IsSimulatingPhysics()) return;
	// 1. 캡슐 콜리전 끄기
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// 2. 메쉬 물리 켜기
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
	// 3. 이동 멈추기
	// 4. 청소 예약
	SetLifeSpan(5.0f);
}

void ABaseEnemy::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// 공격이 끝나면 상태 해제
	bIsAttacking = false;
}

void ABaseEnemy::OnHandOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResul)
{
	// 나 자신이나 같은 적끼리는 때리지 않기
	if (OtherActor == this || OtherActor == GetOwner()) return;
	// 플레이어인지 확인하고 데미지 주기
	if (OtherActor && OtherActor != this)
	{
		UGameplayStatics::ApplyDamage(OtherActor, 10.0f, GetController()
			, this, UDamageType::StaticClass());
	}
}

void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// 감지범위
	DrawDebugSphere(GetWorld(), GetActorLocation(), DetectRange
		, 32, FColor::Yellow, false, -1.0f);
}


float ABaseEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	// 1. 체력감소
	CurrentHealth = FMath::Clamp(CurrentHealth - DamageApplied, 0.f, MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("데미지를 받음 :%f, 남은 체력 :%f"), DamageApplied, CurrentHealth);
	
	// 2. 피격 몽타주 재생
	if (CurrentHealth > 0.f)
	{
		PlayHitMontage(FName("Hit_Back"));
	}
	else
	{
		Die();
		UE_LOG(LogTemp, Error, TEXT("적 사망!"));
	}
	
	// 체력바 갱신
	USoul_Character_HUD* EnemyHUD = Cast<USoul_Character_HUD>(HealthBarWidget->GetUserWidgetObject());
	if (EnemyHUD)
	{
		EnemyHUD->SetHealth(CurrentHealth, MaxHealth);
	}
	if (CurrentHealth <= 0.f)
	{
		HealthBarWidget->SetVisibility(false);
	}
	
	
	return DamageApplied;
}

void ABaseEnemy::Attack()
{
	// 이미 공격중이거나, 맞고있는중이면 공격불가
	if (bIsAttacking || CurrentHealth <= 0.f) return;
	
	if (AttackMontage)
	{
		bIsAttacking = true;
		PlayAnimMontage(AttackMontage);
		// 델리게이트 연결
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			FOnMontageEnded EndDelegate;
			EndDelegate.BindUObject(this, &ABaseEnemy::OnAttackMontageEnded);
			AnimInstance->Montage_SetEndDelegate(EndDelegate);
		}
	}
}

void ABaseEnemy::SetHandCollisionEnabled(bool bEnabled)
{
	if (bEnabled)
	{
		RightHandCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		RightHandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ABaseEnemy::PlayHitMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitMontage)
	{
		AnimInstance->Montage_Play(HitMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitMontage);
	}
	
}


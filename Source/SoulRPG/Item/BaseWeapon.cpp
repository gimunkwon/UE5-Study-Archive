


#include "BaseWeapon.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"


ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	
	// 1. 매쉬 컴포넌트 생성
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	SetRootComponent(ItemMesh);
	// 충돌 설정: 무기는 바닥에 떨어져 있을 때 줍기 위해 겹침(Overlap)만 허용
	ItemMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// 2. 충돌 박스 생성(공격 판정용)
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(GetRootComponent());
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// 겹침이벤트 발생, 물리적 충돌(Block)은 안함
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Overlap);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Ignore); //폰은 무시하도록설정
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	// 충돌 이벤트 바인딩
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ABaseWeapon::OnBoxOverlap);
}

void ABaseWeapon::EnableCollision()
{
	// 이번 공격에서 때린 적 목록 초기화(새 공격)
	IgnoreActors.Empty();
	// 충돌 켜기(QueryOnly: 물리 효과 없이 감지만 함)
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ABaseWeapon::DisableCollision()
{
	// 충돌 끄기
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 1. 나 자신(GetOwner)은 때리면 안됨
	if (OtherActor == GetOwner()) return;
	UE_LOG(LogTemp, Warning, TEXT("충돌 감지됨!: 대상: %s"), *OtherActor->GetName());
	//2. 이미 이번 공격에서 때린 적이면 패스
	if (IgnoreActors.Contains(OtherActor)) return;
	
	// 3. 때린 적 목록에 추가
	IgnoreActors.Add(OtherActor);
	
	
	AController* InstigatorController = nullptr;
	if (GetInstigator())
	{
		InstigatorController = GetInstigator()->GetController();
	}
	else if (GetOwner())
	{
		if (APawn* OwnerPawn = Cast<APawn>(GetOwner()))
		{
			InstigatorController = OwnerPawn->GetController();
		}
	}
	
	
	UGameplayStatics::ApplyDamage(
		OtherActor,
		20.0f,
		GetInstigator()->GetController(),
		this,
		UDamageType::StaticClass());
	
	UE_LOG(LogTemp, Warning, TEXT("적중!: %s"), *OtherActor->GetName());
}

void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// 충돌이 켜져 있을때만(Query Only) 박스를 그립니다.
	if (CollisionBox && CollisionBox->GetCollisionEnabled() == ECollisionEnabled::QueryOnly)
	{
		FVector Center = CollisionBox->GetComponentLocation();
		FVector Extent = CollisionBox->GetScaledBoxExtent();
		FQuat Rotation = CollisionBox->GetComponentQuat();
		
		// 빨간색 박스 그리기
		DrawDebugBox(GetWorld(), Center, Extent,Rotation ,FColor::Red
			, false, -1.0f, 0, 2.0f);
	}
}


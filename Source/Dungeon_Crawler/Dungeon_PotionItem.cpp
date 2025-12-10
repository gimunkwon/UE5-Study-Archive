
#include "Dungeon_PotionItem.h"
#include "Dungeon_Character.h"


#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "InventoryComponent.h"




ADungeon_PotionItem::ADungeon_PotionItem()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// 1. 충돌체 생성
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	RootComponent = SphereComp;
	SphereComp->SetSphereRadius(50.0f);
	SphereComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	//2. 메쉬 생성
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(SphereComp);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	// 1. 회전 컴포넌트 생성
	RotatingComp = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingComp"));
	// 2. 회전 속도 설정(Pitch, Yaw, Roll)
	// Yaw(Z축)으로 초당 180도 회전 (2초에 한 바퀴)
	RotatingComp->RotationRate = FRotator(0.0f, 180.0f, 0.0f);
}

void ADungeon_PotionItem::Interact_Implementation(APawn* InstigatorPawn)
{
	IInteractionInterface::Interact_Implementation(InstigatorPawn);
	
	// 플레이어 인지 확인
	if (InstigatorPawn)
	{
		ADungeon_Character* Player = Cast<ADungeon_Character>(InstigatorPawn);
		if (Player)
		{
			// 1. 플레이어의 인벤토리 가져오기
			UInventoryComponent* Inventory = Player->GetInventory();
			if (Inventory)
			{
				// 2. 인벤토리에 내 정보를 넘겨줌
				Inventory->AddItem(ItemInfo);
				
				UE_LOG(LogTemp, Warning, TEXT("E키로 아이템 흭득!"));
				Destroy();
			}
		}
		
	}
	
}


void ADungeon_PotionItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADungeon_PotionItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


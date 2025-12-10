// Fill out your copyright notice in the Description page of Project Settings.


#include "Dungeon_Portal.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

// Sets default values
ADungeon_Portal::ADungeon_Portal()
{
	PrimaryActorTick.bCanEverTick = false;
	
	// 1. 컴포넌트 생성
	TransferVolume = CreateDefaultSubobject<UBoxComponent>("TransferVolumeBoxComp");
	RootComponent = TransferVolume; // 박스를 루트로 (배치 편하게)
	TransferVolume->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));
	TransferVolume->SetCollisionProfileName(TEXT("Trigger")); // 겹침 갑지용
	
	PortalMesh = CreateDefaultSubobject<UStaticMeshComponent>("PortalMesh");
	PortalMesh->SetupAttachment(RootComponent);
	PortalMesh->SetCollisionProfileName(TEXT("NoCollision")); // 문틀은 통과 가능하게
	
	PortalEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PortalEffect"));
	PortalEffect->SetupAttachment(RootComponent);
	
	// 2. 이벤트 연결
	TransferVolume->OnComponentBeginOverlap.AddDynamic(this, &ADungeon_Portal::OnOverlapBegin);
}

// Called when the game starts or when spawned
void ADungeon_Portal::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADungeon_Portal::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 캐릭터가 닿았는지 확인
	if (OtherActor && Cast<ACharacter>(OtherActor))
	{
		// 맵 이름이 비어있지 않다면 이동!
		if (!TransferLevelName.IsNone())
		{
			UE_LOG(LogTemp, Warning, TEXT("포탈 작동! %s(으)로 이동합니다."), *TransferLevelName.ToString())
			
			// 레벨 열기(이동)
			UGameplayStatics::OpenLevel(this, TransferLevelName);
		}
	}
}

// Called every frame
void ADungeon_Portal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


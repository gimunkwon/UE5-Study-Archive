// Fill out your copyright notice in the Description page of Project Settings.


#include "TreasureChest.h"

#include "Components/StaticMeshComponent.h"


// Sets default values
ATreasureChest::ATreasureChest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	// 1. 몸통 생성
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;
	
	// 뚜껑 생성
	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);
}

// Called when the game starts or when spawned
void ATreasureChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATreasureChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATreasureChest::Interact_Implementation(APawn* InstigatorPawn)
{
	IInteractionInterface::Interact_Implementation(InstigatorPawn);
	
	// 이미 열렸으면 무시
	if (bIsOpened) return;
	
	// 1. 상태 변경
	bIsOpened = true;
	UE_LOG(LogTemp, Warning, TEXT("상자 열림"));
	
	// 2. 블루프린트에게 "뚜껑 열리는 애니메이션 재생해"라고 신호 보냄
	OnChestOpened();
	
	// 3. 아이템소환 (보상)
	for (const TSubclassOf<AActor>& ItemClass : ItemClassToSpawn)
	{
		if (ItemClass)
		{
			// 상자 위쪽, 살짝 랜덤한 위치에 소환
			FVector SpawnLoc = GetActorLocation() + FVector(0,0,100.0f);
			SpawnLoc.X += FMath::RandRange(-50.0f, 50.0f);
			SpawnLoc.Y += FMath::RandRange(-50.0f, 50.0f);
			
			GetWorld()->SpawnActor<AActor>(ItemClass, SpawnLoc, FRotator::ZeroRotator);
			
		}
	}
}


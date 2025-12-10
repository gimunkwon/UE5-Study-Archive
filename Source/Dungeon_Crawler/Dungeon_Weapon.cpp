// Fill out your copyright notice in the Description page of Project Settings.


#include "Dungeon_Weapon.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

#include "Kismet/GameplayStatics.h"


// Sets default values
ADungeon_Weapon::ADungeon_Weapon()
{
	PrimaryActorTick.bCanEverTick = false;
	
	// 1. 매쉬를 루트로 설정 (칼이 곧 몸통)
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;
	WeaponMesh->SetCollisionProfileName(TEXT("NoCollision")); // 평소에 충돌끄기(주인 몸에 걸리니까)
	
	// 2. 공격 판정 박스
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(WeaponMesh);
	// 충돌 박스 설정(처음에 꺼둠)
	CollisionBox->SetCollisionProfileName(TEXT("NoCollision"));
	
}

void ADungeon_Weapon::EnableCollision()
{
	// 충돌 켜기 (QueryOnly: 물리엔진은 안 쓰고 충돌검사만 함)
	if (CollisionBox)
	{
		UE_LOG(LogTemp,Warning,TEXT("EnableCollision"));
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
		CollisionBox->SetGenerateOverlapEvents(true);
	
		CollisionBox->UpdateOverlaps();		
	}
	
}

void ADungeon_Weapon::DisableCollision()
{
	if (CollisionBox)
	{
		// 충돌 끄기
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CollisionBox->SetGenerateOverlapEvents(false);
	}
	
}


void ADungeon_Weapon::BeginPlay()
{
	Super::BeginPlay();
	
	if (CollisionBox)
	{
		CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ADungeon_Weapon::OnBoxOverlap);
	}
	
}


void ADungeon_Weapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 내 캐릭터는 때리면 안됨
	if (OtherActor && (OtherActor != GetOwner()))
	{
		// 1. 이미 때린 놈인지 확인하는 리스트가 필요하지만(다단히트 방지), 지금은 일단 그냥 때림
		UE_LOG(LogTemp, Warning, TEXT("타격 성공!"));
		
		// 2. 데미지 적용
		UGameplayStatics::ApplyDamage(OtherActor, 100.0f, GetInstigatorController()
			, this, UDamageType::StaticClass());
		
	}
	
}

void ADungeon_Weapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


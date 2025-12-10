// Fill out your copyright notice in the Description page of Project Settings.


#include "Dungeon_DamageTextActor.h"
#include "Dungeon_DamageTextWidget.h"

#include "Components/WidgetComponent.h"


// Sets default values
ADungeon_DamageTextActor::ADungeon_DamageTextActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	DamageWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("DamageWidgetComp"));
	RootComponent = DamageWidgetComp;
	
	// 위젯 설정 (빌보드 모드: 항상 카메라를 봄)
	DamageWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	DamageWidgetComp->SetDrawSize(FVector2D(200.0f, 50.0f));
	
	// 1초 뒤에 자동 삭제
	InitialLifeSpan = 1.0f;
}

// Called when the game starts or when spawned
void ADungeon_DamageTextActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADungeon_DamageTextActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// 매 프레임 위로(Z축) 이동
	FVector NewLocation = GetActorLocation();
	NewLocation.Z += FloatSpeed * DeltaTime;
	SetActorLocation(NewLocation);
}

void ADungeon_DamageTextActor::SetDamage(float DamageAmount)
{
	if (DamageWidgetComp)
	{
		// 컴포넌트 안에 있는 실제 위젯(UserWidget)을 꺼내서 우리 클래스로 형변환
		UDungeon_DamageTextWidget* Widget = Cast<UDungeon_DamageTextWidget>(DamageWidgetComp->GetUserWidgetObject());
		if (Widget)
		{
			Widget->SetDamageText(DamageAmount);
		}
	}
}


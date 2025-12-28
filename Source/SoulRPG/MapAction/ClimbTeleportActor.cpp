


#include "ClimbTeleportActor.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Evaluation/IMovieSceneEvaluationHook.h"
#include "GameFramework/Character.h"


AClimbTeleportActor::AClimbTeleportActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;
	// 트리거 박스
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(RootComponent);
	TriggerBox->SetBoxExtent(FVector(100.f, 100.f, 100.f));
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	// 도착 지점 화살표 생성
	DestPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("DestPoint"));
	DestPoint->SetupAttachment(RootComponent);
	DestPoint->SetRelativeLocation(FVector(0.f,0.f,200.f));
	DestPoint->ArrowColor = FColor::Cyan;
	
	TargetrCharacter = nullptr;
}

void AClimbTeleportActor::BeginPlay()
{
	Super::BeginPlay();
	
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AClimbTeleportActor::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AClimbTeleportActor::OnOverlapEnd);
}

void AClimbTeleportActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AClimbTeleportActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResul)
{
	ACharacter* Char = Cast<ACharacter>(OtherActor);
	if (Char)
	{
		TargetrCharacter = Char;
		APlayerController* PC = Cast<APlayerController>(Char->GetController());
		if (PC)
		{
			// 이 액터에 대해 입력 권한 부여
			EnableInput(PC);
			
			// 매핑 컨텍스트 등록
			if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
			{
				if (UEnhancedInputLocalPlayerSubsystem* SubSystem =
					LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
				{
					SubSystem->AddMappingContext(DefaultContext, 2);
				}
			}
			
			
			
			if (UEnhancedInputComponent* EnhancedInput = 
				Cast<UEnhancedInputComponent>(InputComponent))
			{
				EnhancedInput->BindAction(InteractAction, ETriggerEvent::Started, this, &AClimbTeleportActor::OnInteract);
			}
			UE_LOG(LogTemp, Warning, TEXT("텔레포트 영역 진입 E키를 누르세요"))
		}
	}
}

void AClimbTeleportActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == TargetrCharacter)
	{
		APlayerController* PC = Cast<APlayerController>(TargetrCharacter->GetController());
		if (PC)
		{
			// 매핑 컨텍스트 제거
			if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
			{
				if (UEnhancedInputLocalPlayerSubsystem* SubSystem =
					LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
				{
					SubSystem->RemoveMappingContext(DefaultContext);
				}
			}
			
			
			DisableInput(PC);
		}
		TargetrCharacter = nullptr;
		UE_LOG(LogTemp, Warning, TEXT("텔레포트 영역 이탈"));
	}
}

// E키 눌렀을 때 (1단계 : 화면 어둡게 + 타이머 시작)
void AClimbTeleportActor::OnInteract()
{
	
	if (!TargetrCharacter) return;
	
	APlayerController* PC = Cast<APlayerController>(TargetrCharacter->GetController());
	if (PC && PC->PlayerCameraManager)
	{
		TargetrCharacter->DisableInput(PC);
		PC->PlayerCameraManager->StartCameraFade(0.f, 1.f, FadeDuration, FLinearColor::Black, false, true);
		// FadeDuration 후에 실제 이동 함수 호출
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_Teleport, this
			, &AClimbTeleportActor::PerformTeleport,FadeDuration, false);
	}
}

// 화면이 다 어두워진 뒤 실행(2단계 : 이동 + 화면 밝게)
void AClimbTeleportActor::PerformTeleport()
{
	ACharacter* TempChar = TargetrCharacter;
	
	
	if (!TargetrCharacter) return;
	// 위치 및 회전 이동
	// DestPoint의 월드 위치로 이동
	if (!DestPoint)
	{
		UE_LOG(LogTemp, Error, TEXT("DestPoint가 없음"));
		return;
	}
	
	
	TempChar->SetActorLocation(DestPoint->GetComponentLocation());
	TempChar->SetActorRotation(DestPoint->GetComponentRotation());
	// 컨트롤러 회전도 맞춰주기
	APlayerController* PC = Cast<APlayerController>(TempChar->GetController());
	if (PC)
	{
		PC->SetControlRotation(DestPoint->GetComponentRotation());
		// 입력 다시 풀기
		TempChar->EnableInput(PC);
		// 화면 밝게
		if (PC->PlayerCameraManager)
		{
			PC->PlayerCameraManager->StartCameraFade(1.f,0.f,FadeDuration, FLinearColor::Black, false, false);
		}
	}
}


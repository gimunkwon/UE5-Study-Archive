// Fill out your copyright notice in the Description page of Project Settings.


#include "Dungeon_NPC.h"

#include "Dungeon_Character.h"
#include "InventoryComponent.h"
#include "Dungeon_ItemStruct.h"

#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ADungeon_NPC::ADungeon_NPC()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADungeon_NPC::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADungeon_NPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ADungeon_NPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ADungeon_NPC::Interact_Implementation(APawn* InstigatorPawn)
{
	IInteractionInterface::Interact_Implementation(InstigatorPawn);
#pragma region DialogueWidget
	if (!InstigatorPawn) return;
	
	// 1. 인사 로그 띄우기 (나중에 대화창 UI로 바뀔 부분)
	UE_LOG(LogTemp, Warning, TEXT("NPC : 반갑네, 모험가여 마을에 온 걸 환영하네"));
	
	// 2. 플레이어 쳐다보기
	// 내 위치(Start)에서 플레이어 위치(Target)를 바라보는 회전값 계산
	FVector Start = GetActorLocation();
	FVector Target = InstigatorPawn->GetActorLocation();
	
	// z축(높이)는 무시하고 수평으로만 돌게 함
	Start.Z = 0;
	Target.Z = 0;
	
	FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(Start, Target);
	
	// 즉시 회전 (나중에 RInterpTo를 쓰면 부드럽게 돌아감)
	SetActorRotation(LookAtRot);
	
	// 1. 위젯 생성 (없을 때만)
	if (!DialogueInstance && DialogueWidgetClass)
	{
		DialogueInstance = CreateWidget<UDungeon_DialogueWidget>(GetWorld(), DialogueWidgetClass);
	}
	
	// 2. 위젯 띄우기 및 내용 설정
	if (DialogueInstance)
	{
		DialogueInstance->AddToViewport();
		DialogueInstance->UpdateDialogue(TEXT("촌장"), TEXT("자네가 그 소문의 그 모험가인가\n던전은 마을 북쪽에 있다네. 조심하게나"));
		
		// 마우스 커서 보이게 하기
		if (APlayerController* PC = Cast<APlayerController>(InstigatorPawn->GetController()))
		{
			// UI만 조작 가능하게 변경
			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(DialogueInstance->TakeWidget()); // 포커스를 위젯으로
			PC->SetInputMode(InputMode);
			
			PC->SetShowMouseCursor(true);
			
			// 혹시 걷고 있다면 강제로 멈추기 (관성제거)
			InstigatorPawn->GetMovementComponent()->StopMovementImmediately();
		}
	}
#pragma endregion
	ADungeon_Character* Player = Cast<ADungeon_Character>(InstigatorPawn);
	if (!Player) return;
	
	UQuestComponent* Quest = Player->GetQuestComponent();
	UInventoryComponent* Inventory = Player->GetInventory();
	
	FString DialogueText = TEXT("");
	
	if (Quest)
	{
		// 상황 1: 이미 퀘스트를 완료함
		if (Quest->bIsCompleted)
		{
			DialogueText = TEXT("마을이 평화로워졌네. 고맙네!");
		}
		// 상황 2: 퀘스트 진행 중
		else if (Quest->bHasQuest)
		{
			// 목표 달성여부
			if (Quest->CurrentKills >= Quest->TargetKills)
			{
				DialogueText = TEXT("오오! 적들을 다 물리쳤군. 여기 보상일세");
				
				// 1. 인벤토리가 있고, 보상 아이템의 이름이 비어있지 않다면
				if (Inventory && !QuestRewardItem.ItemName.IsEmpty())
				{
					// 2. 인벤토리에 구조체를 그대로 전달
					Inventory->AddItem(QuestRewardItem);
					
					UE_LOG(LogTemp, Warning, TEXT("퀘스트 보상 지급 : %s"), *QuestRewardItem.ItemName.ToString());
				}
				
				
				
				Quest->FinishQuest();
			}
			else
			{
				// 아직 덜잡음
				DialogueText = FString::Printf(TEXT("아직 %d마리밖에 못 잡았군. 3마리를 채워오게나"),Quest->CurrentKills);
			}
		}
		//상황 3: 퀘스트 시작 전 (처음 말 걸음)
		else
		{
			DialogueText = TEXT("마을 주변에 몬스터가 너무많아.\n자네가 3마리만 잡아줄수 있겠나");
			Quest->StartQuest();
		}
	}
	// 대화창 업데이트
	if (DialogueInstance)
	{
		DialogueInstance->AddToViewport();
		DialogueInstance->UpdateDialogue(TEXT("촌장"), DialogueText);
	}
}






// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie_GameOverWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UZombie_GameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (RestartBtn)
	{
		RestartBtn->OnClicked.AddDynamic(this, &UZombie_GameOverWidget::OnRestartClicked);
	}
	
	// if (MainMenuBtn)
	// {
	// 	MainMenuBtn->OnClicked.AddDynamic(this, &UZombie_GameOverWidget::OnMainMenuClicked);
	// }
}

void UZombie_GameOverWidget::OnRestartClicked()
{
	// 현재 레벨의 이름을 가져와서 다시 염
	FString CurrentLevelName = GetWorld()->GetMapName();
	
	// 에디터에서는 이름 앞에 "UEDPIE_0_"같은 게 붙어서 이를 제거해주는 작업
	CurrentLevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	
	UGameplayStatics::OpenLevel(this, FName(*CurrentLevelName));
}

void UZombie_GameOverWidget::OnMainMenuClicked()
{
}

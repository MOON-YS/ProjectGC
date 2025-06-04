// Fill out your copyright notice in the Description page of Project Settings.


#include "GCGameState.h"
#include "GCGameInstance.h"
#include "Components/Widget.h"
#include "Engine/GameViewportClient.h"
#include "GCUIResult.h"
AGCGameState::AGCGameState()
{
	MaxHp = 100;
	CurrentHp = MaxHp;
	Level = 1;
	CurrentExp = 0;
	Credit = 100;
	CurrentPrice = 30;
	TimeRemain = 600;
	MaxExpCurrentLevel = 100;
	KillScore = 0;
	EnemySpeedScale = 1.0f;
	EnemySpawnScale = 1;
	static ConstructorHelpers::FClassFinder<UGCUIResult> UI_Result_C(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/UI_End.UI_End_C'"));
	if (UI_Result_C.Succeeded())
	{
		ResultUIClass = UI_Result_C.Class;
	}

}

void AGCGameState::SetGameEnd()
{
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	GetWorld()->GetTimerManager().ClearTimer(TimeHandle);
	ResultUI = CreateWidget<UGCUIResult>(GetWorld()->GetFirstPlayerController(), ResultUIClass);
	ResultUI->SetResultData(TimeRemain,KillScore);
	FInputModeUIOnly Input;
	GetWorld()->GetFirstPlayerController()->SetInputMode(Input);
	ResultUI->SetVisibility(ESlateVisibility::Visible);
	ResultUI->AddToViewport(5);
}

void AGCGameState::SetCurrentHp(float NewHp)
{
	CurrentHp = NewHp;
	OnHpChanged.Broadcast();
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		SetGameEnd();
	}
	

}

void AGCGameState::AddExp(int32 AddedExp)
{
	
	CurrentExp += AddedExp;
	KillScore++;
	if (CurrentExp >= MaxExpCurrentLevel)
	{
		Level++;
		CurrentExp -= MaxExpCurrentLevel;
		MaxExpCurrentLevel = MaxExpCurrentLevel*1.2;
		FInputModeUIOnly InputMode;
		GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
		UGameplayStatics::SetGamePaused(GetWorld(),true);
		OnLevelChanged.Broadcast();

	}
	OnExpChanged.Broadcast();

}

void AGCGameState::LevelUp()
{
}

bool AGCGameState::IsPossibleToBuy()
{
	if (Credit >= CurrentPrice)
	{
		return true;
	}
	return false;
}

void AGCGameState::AddCredit(int32 AddedCredit)
{
	Credit += AddedCredit;
	OnCreditChanged.Broadcast();
}

void AGCGameState::Purchase()
{
	Credit -= CurrentPrice;
	CurrentPrice += 15;
	OnCreditChanged.Broadcast();
}	

void AGCGameState::TimeFolw()
{
	TimeRemain--;
	OnTimeFlow.Broadcast();
	if (TimeRemain == 0)
	{
		SetGameEnd();
	}
	if (TimeRemain % 120 == 0)
	{
		EnemySpeedScale += 0.125f;
	}
	if (TimeRemain % 180 == 0)
	{
		EnemySpawnScale++;
	}
}

void AGCGameState::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(TimeHandle, this, &AGCGameState::TimeFolw, 1.0f, true);

	auto GameInstance = Cast<UGCGameInstance>(GetGameInstance());
	GameInstance->ResetData();

	
}

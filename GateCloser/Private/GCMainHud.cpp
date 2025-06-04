// Fill out your copyright notice in the Description page of Project Settings.


#include "GCMainHud.h"
#include "GCGameState.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UGCMainHud::NativeConstruct()
{
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HpBar")));
	GCCHECK(HPProgressBar != nullptr);
	ExpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_ExpBar")));
	GCCHECK(ExpProgressBar != nullptr);
	TimeCounter = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_Time")));
	GCCHECK(TimeCounter != nullptr);
	Credit = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_Credit")));
	GCCHECK(Credit != nullptr);
	KillScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("TB_KillScore")));
	GCCHECK(Credit != nullptr);

}

void UGCMainHud::UpdateTime()
{
	int32 Seconds = GameState->GetTimeRemain();
	int32 Minute = Seconds / 60;
	int32 SecondsRemain = Seconds % 60;

	TimeCounter->SetText(FText::FromString(FString::Printf(TEXT("%d:%02d"), Minute, SecondsRemain)));
}

void UGCMainHud::UpdateHp()
{
	HPProgressBar->SetPercent(GameState->GetCurrentHpRatio());
}

void UGCMainHud::UpdateState()
{
	Credit->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), GameState->GetCurrentCredit(), GameState->GetCurrentPrice())));
	if (!GameState->IsPossibleToBuy())
	{
		FSlateColor NewColor = FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
		Credit->SetColorAndOpacity(NewColor);
	}
	else
	{
		FSlateColor NewColor = FSlateColor(FLinearColor(0.0f, 1.0f, 0.0f, 1.0f));
		Credit->SetColorAndOpacity(NewColor);
	}
	KillScore->SetText(FText::FromString(FString::FromInt(GameState->GetCurrentScore())));
	ExpProgressBar->SetPercent(GameState->GetCurrentExpRatio());
}


void UGCMainHud::BindGameState()
{	
	GameState = Cast<AGCGameState>(UGameplayStatics::GetGameState(GetWorld()));;
	GameState->OnTimeFlow.AddUObject(this, &UGCMainHud::UpdateTime);
	GameState->OnHpChanged.AddUObject(this, &UGCMainHud::UpdateHp);
	GameState->OnExpChanged.AddUObject(this, &UGCMainHud::UpdateState);
	GameState->OnCreditChanged.AddUObject(this, &UGCMainHud::UpdateState);
}

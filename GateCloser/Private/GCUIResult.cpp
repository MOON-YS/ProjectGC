// Fill out your copyright notice in the Description page of Project Settings.


#include "GCUIResult.h"
#include "Components/TextBlock.h"
#include "GCGameState.h"
void UGCUIResult::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGCUIResult::SetResultData(int32 TimeRemain,int32 Score)
{
	Results = Cast<UTextBlock>(GetWidgetFromName(TEXT("TB_Result")));
	KillScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("TB_KillScore")));
	SurviveTime = Cast<UTextBlock>(GetWidgetFromName(TEXT("TB_SurviveTime")));

	if (Results != nullptr) 
	{
		if (TimeRemain == 0)
		{
			Results->SetText(FText::FromString("Mission Success!"));
		}
		else
		{
			Results->SetText(FText::FromString("Mission Failed.."));
		}
	}

	int32 SurvivedTime = 600 - TimeRemain;
	int32 Minute = SurvivedTime / 60;
	int32 SecondsRemain = SurvivedTime % 60;

	if(SurviveTime != nullptr)\
		SurviveTime->SetText(FText::FromString(FString::Printf(TEXT("%d:%02d"), Minute, SecondsRemain)));

	if (KillScore != nullptr)\
		KillScore->SetText(FText::FromString(FString::Printf(TEXT("%d"), Score)));
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "GCUIBuffs.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "GCGameState.h"
#include "GCGameInstance.h"
void UGCUIBuffs::NativeConstruct()
{
	Close = Cast<UButton>(GetWidgetFromName(TEXT("BTN_Close")));
	if (Close != nullptr)
	{
		Close->OnClicked.AddDynamic(this, &UGCUIBuffs::OnCloseButtonClicked);
	}
	List.Add(TPair<EUnitType,FString>(EUnitType::GATECLOSER, TEXT("GC")));
	List.Add(TPair<EUnitType,FString>(EUnitType::M1A1, TEXT("M1A1")));
	List.Add(TPair<EUnitType,FString>(EUnitType::MARLIN, TEXT("Marlin")));
	List.Add(TPair<EUnitType,FString>(EUnitType::CIWS, TEXT("CIWS")));
	List.Add(TPair<EUnitType,FString>(EUnitType::SEARAM, TEXT("RAM")));
	List.Add(TPair<EUnitType,FString>(EUnitType::PATRIOT, TEXT("Patriot")));
	List.Add(TPair<EUnitType,FString>(EUnitType::M777, TEXT("M777")));
}

void UGCUIBuffs::Update()
{
	auto GameInstance = Cast<UGCGameInstance>(GetGameInstance());
	for (TPair Item : List)
	{
		
		UTextBlock* AD = Cast<UTextBlock>(GetWidgetFromName(FName(*FString::Printf(TEXT("TB_%s_AD"),*Item.Value))));
		if (AD == nullptr) 
		{
			GCLOG(Warning, TEXT("TB_%s_AD is nullptr"), *Item.Value);
		}
		UTextBlock* ACR = Cast<UTextBlock>(GetWidgetFromName(FName(*FString::Printf(TEXT("TB_%s_ACR"), *Item.Value))));
		if (ACR == nullptr)
		{
			GCLOG(Warning, TEXT("TB_%s_ACR is nullptr"), *Item.Value);
		}
		UTextBlock* AR = Cast<UTextBlock>(GetWidgetFromName(FName(*FString::Printf(TEXT("TB_%s_AR"), *Item.Value))));
		if (AR == nullptr)
		{
			GCLOG(Warning, TEXT("TB_%s_AR is nullptr"), *Item.Value);
		}
		UTextBlock* AP = Cast<UTextBlock>(GetWidgetFromName(FName(*FString::Printf(TEXT("TB_%s_AP"), *Item.Value))));
		if (AP == nullptr)
		{
			GCLOG(Warning, TEXT("TB_%s_AP is nullptr"), *Item.Value);
		}

		FGCUnitBuffData* Data = GameInstance->GetUnitBuffData(Item.Key);
		AD->SetText(FText::FromString(FString::Printf(TEXT("+%d%%"), FMath::RoundToInt(Data->AttackScale * 100) - 100)));
		ACR->SetText(FText::FromString(FString::Printf(TEXT("-%d%%"), 100 - FMath::RoundToInt(Data->AttackCoolScale * 100))));
		AR->SetText(FText::FromString(FString::Printf(TEXT("+%d%%"), FMath::RoundToInt(Data->RangeBonus * 100) - 100)));
		if (Item.Key == EUnitType::SEARAM)
		{
			AP->SetText(FText::FromString(FString::Printf(TEXT("+%d"), Data->AdditionalProjectile)));
		}
		else
		{
			AP->SetText(FText::FromString(FString::Printf(TEXT(""))));
		}
		if (Item.Key == EUnitType::CIWS)
		{
			ACR->SetText(FText::FromString(FString::Printf(TEXT(""))));
		}
		if (Item.Key == EUnitType::GATECLOSER || Item.Key == EUnitType::SEARAM || Item.Key  == EUnitType::PATRIOT)
		{
			AR->SetText(FText::FromString(FString::Printf(TEXT(""))));
		}

	}

}


void UGCUIBuffs::OnCloseButtonClicked()
{
	FInputModeGameOnly InputMode;
	GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
	SetVisibility(ESlateVisibility::Hidden);
}

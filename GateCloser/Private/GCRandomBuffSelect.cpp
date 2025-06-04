// Fill out your copyright notice in the Description page of Project Settings.


#include "GCRandomBuffSelect.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "GCGameInstance.h"
#include "GCGameState.h"
void UGCRandomBuffSelect::NativeConstruct()
{
	Super::NativeConstruct();

	ItemDictionary.Add(EUnitType::CIWS, TEXT("CIWS"));
	ItemDictionary.Add(EUnitType::M1A1, TEXT("M1A1"));
	ItemDictionary.Add(EUnitType::M777, TEXT("M777"));
	ItemDictionary.Add(EUnitType::SEARAM, TEXT("RIM-116"));
	ItemDictionary.Add(EUnitType::MARLIN, TEXT("Marlin"));
	ItemDictionary.Add(EUnitType::GATECLOSER, TEXT("GateCloser"));

	EnhanceCode.Add(0, TEXT("Attack Damage +25%"));
	EnhanceCode.Add(1, TEXT("Attack Cool -10%"));
	EnhanceCode.Add(2, TEXT("Attack Range +100%"));
	EnhanceCode.Add(3, TEXT("Projectile +1"));

	Item1 = Cast<UButton>(GetWidgetFromName(TEXT("BTN_Item1")));
	if (Item1 != nullptr)
	{
		Item1->OnClicked.AddDynamic(this, &UGCRandomBuffSelect::OnButton1Clicked);
	}

	Item1_Name = Cast<UTextBlock>(GetWidgetFromName(TEXT("TB_Item1_Name")));
	Item1_Des = Cast<UTextBlock>(GetWidgetFromName(TEXT("TB_Item1_Describtion")));
	Item1_CurrentState = Cast<UTextBlock>(GetWidgetFromName(TEXT("TB_Item1_Current")));

	Item2 = Cast<UButton>(GetWidgetFromName(TEXT("BTN_Item2")));
	if (Item2 != nullptr)
	{
		Item2->OnClicked.AddDynamic(this, &UGCRandomBuffSelect::OnButton2Clicked);
	}

	Item2_Name = Cast<UTextBlock>(GetWidgetFromName(TEXT("TB_Item2_Name")));
	Item2_Des = Cast<UTextBlock>(GetWidgetFromName(TEXT("TB_Item2_Describtion")));
	Item2_CurrentState = Cast<UTextBlock>(GetWidgetFromName(TEXT("TB_Item2_Current")));

	Item3 = Cast<UButton>(GetWidgetFromName(TEXT("BTN_Item3")));
	if (Item3 != nullptr)
	{
		Item3->OnClicked.AddDynamic(this, &UGCRandomBuffSelect::OnButton3Clicked);

	}

	Item3_Name = Cast<UTextBlock>(GetWidgetFromName(TEXT("TB_Item3_Name")));
	Item3_Des = Cast<UTextBlock>(GetWidgetFromName(TEXT("TB_Item3_Describtion")));
	Item3_CurrentState = Cast<UTextBlock>(GetWidgetFromName(TEXT("TB_Item3_Current")));
}

void UGCRandomBuffSelect::OnButton1Clicked()
{
	auto GameInstance = Cast<UGCGameInstance>(GetGameInstance());
	switch (Choices[0].Value) 
	{
	case 0 :
	{
		GameInstance->AddAttackScale(Choices[0].Key,0.25);
		break;
	}
	case 1:
	{
		GameInstance->AddAttackCoolScale(Choices[0].Key, 0.1);
		break;
	}
	case 2:
		GameInstance->AddRangeScale(Choices[0].Key, 1.0f);
		break;
	case 3:
		GameInstance->AddProjectile(Choices[0].Key);
		break;
	default:
		break;
	}
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	SetVisibility(ESlateVisibility::Hidden);
	FInputModeGameOnly InputMode;
	GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
}

void UGCRandomBuffSelect::OnButton2Clicked()
{
	auto GameInstance = Cast<UGCGameInstance>(GetGameInstance());
	switch (Choices[1].Value)
	{
	case 0:
	{
		GameInstance->AddAttackScale(Choices[1].Key, 0.25f);
		break;
	}
	case 1:
	{
		GameInstance->AddAttackCoolScale(Choices[1].Key, 0.1f);
		break;
	}
	case 2:
		GameInstance->AddRangeScale(Choices[1].Key, 1.0f);
		break;
	case 3:
		GameInstance->AddProjectile(Choices[1].Key);
		break;
	default:
		break;
	}
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	SetVisibility(ESlateVisibility::Hidden);
	FInputModeGameOnly InputMode;
	GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;

}

void UGCRandomBuffSelect::OnButton3Clicked()
{
	auto GameInstance = Cast<UGCGameInstance>(GetGameInstance());
	switch (Choices[2].Value)
	{
	case 0:
	{
		GameInstance->AddAttackScale(Choices[2].Key, 0.25f);
		break;
	}
	case 1:
	{
		GameInstance->AddAttackCoolScale(Choices[2].Key, 0.1f);
		break;
	}
	case 2:
		GameInstance->AddRangeScale(Choices[2].Key, 1.0f);
		break;
	case 3:
		GameInstance->AddProjectile(Choices[2].Key);
		break;
	default:
		break;
	}
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	SetVisibility(ESlateVisibility::Hidden);
	FInputModeGameOnly InputMode;
	GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;

}

void UGCRandomBuffSelect::Roll()
{
	/*
	0 : AttackScale;
	1 : AttackCoolScale;
	2 : RangeScale;
	3 : AdditionalProjectile
	*/
	TArray<TPair<EUnitType, int32>> RandomTable;
	RandomTable.Add(TPair<EUnitType, int32>(EUnitType::M777, 0));
	RandomTable.Add(TPair<EUnitType, int32>(EUnitType::M777, 2));
	RandomTable.Add(TPair<EUnitType, int32>(EUnitType::M1A1, 0));
	RandomTable.Add(TPair<EUnitType, int32>(EUnitType::M1A1, 2));
	RandomTable.Add(TPair<EUnitType, int32>(EUnitType::CIWS, 0));
	RandomTable.Add(TPair<EUnitType, int32>(EUnitType::CIWS, 2));
	RandomTable.Add(TPair<EUnitType, int32>(EUnitType::SEARAM, 0));
	RandomTable.Add(TPair<EUnitType, int32>(EUnitType::MARLIN, 0));
	RandomTable.Add(TPair<EUnitType, int32>(EUnitType::MARLIN, 2));
	RandomTable.Add(TPair<EUnitType, int32>(EUnitType::GATECLOSER, 0));

	auto GameInstance = Cast<UGCGameInstance>(GetGameInstance());
	FGCUnitBuffData* Data = GameInstance->GetUnitBuffData(EUnitType::SEARAM);
	if (Data->AdditionalProjectile < 8) 
	{
		RandomTable.Add(TPair<EUnitType, int32>(EUnitType::SEARAM, 3));
	}
	Data = GameInstance->GetUnitBuffData(EUnitType::M777);
	if (FMath::RoundToInt(Data->AttackCoolScale * 100) > 50)
	{
		RandomTable.Add(TPair<EUnitType, int32>(EUnitType::M777, 1));
	}
	Data = GameInstance->GetUnitBuffData(EUnitType::M1A1);
	if (FMath::RoundToInt(Data->AttackCoolScale * 100) > 50)
	{
		RandomTable.Add(TPair<EUnitType, int32>(EUnitType::M1A1, 1));
	}
	Data = GameInstance->GetUnitBuffData(EUnitType::SEARAM);
	if (FMath::RoundToInt(Data->AttackCoolScale * 100) > 50)
	{
		RandomTable.Add(TPair<EUnitType, int32>(EUnitType::SEARAM, 1));
	}
	Data = GameInstance->GetUnitBuffData(EUnitType::MARLIN);
	if (FMath::RoundToInt(Data->AttackCoolScale * 100) > 50)
	{
		RandomTable.Add(TPair<EUnitType, int32>(EUnitType::MARLIN, 1));
	}
	Data = GameInstance->GetUnitBuffData(EUnitType::GATECLOSER);
	if (FMath::RoundToInt(Data->AttackCoolScale * 100) > 50)
	{
		RandomTable.Add(TPair<EUnitType, int32>(EUnitType::GATECLOSER, 1));
	}

	Choices.Empty();
	for (int32 i = 0; i < 3; i++)
	{
		int32 RandomIndex = FMath::RandRange(0, RandomTable.Num() - 1);
		Choices.Add(RandomTable[RandomIndex]);
		RandomTable.RemoveAt(RandomIndex);
	}

	FString* ItemName1 = ItemDictionary.Find(Choices[0].Key);
	Item1_Name->SetText(FText::FromString(*ItemName1));
	FString* ItemDescrib1 = EnhanceCode.Find(Choices[0].Value);
	Item1_Des->SetText(FText::FromString(*ItemDescrib1));

	int32 Num = 0;
	FGCUnitBuffData* DataTmp = GameInstance->GetUnitBuffData(Choices[0].Key);
	switch (Choices[0].Value)
	{
	case 0:
	{
		Num = FMath::RoundToInt(DataTmp->AttackScale * 100) - 100;
		Item1_CurrentState->SetText(FText::FromString(FString::Printf(TEXT("Current Added +%d%%"), Num)));
		break;
	}
	case 1:
	{
		Num = 100 - FMath::RoundToInt(DataTmp->AttackCoolScale * 100);
		Item1_CurrentState->SetText(FText::FromString(FString::Printf(TEXT("Current Reduce -%d%%"), Num)));
		break;
	}
	case 2:
	{
		Num = FMath::RoundToInt(DataTmp->RangeBonus * 100) - 100;
		Item1_CurrentState->SetText(FText::FromString(FString::Printf(TEXT("Current Added +%d%%"), Num)));
		break;
	}
	case 3:
	{
		Item1_CurrentState->SetText(FText::FromString(FString::Printf(TEXT("Current Added +%d"), DataTmp->AdditionalProjectile)));
		break;
	}
	default:
		break;
	}

	FString* ItemName2 = ItemDictionary.Find(Choices[1].Key);
	Item2_Name->SetText(FText::FromString(*ItemName2));
	FString* ItemDescrib2 = EnhanceCode.Find(Choices[1].Value);
	Item2_Des->SetText(FText::FromString(*ItemDescrib2));
	DataTmp = GameInstance->GetUnitBuffData(Choices[1].Key);
	switch (Choices[1].Value)
	{
	case 0:
	{
		Num = FMath::RoundToInt(DataTmp->AttackScale * 100) - 100;
		Item2_CurrentState->SetText(FText::FromString(FString::Printf(TEXT("Current Added +%d%%"), Num)));
		break;
	}
	case 1:
	{
		Num = 100 - FMath::RoundToInt(DataTmp->AttackCoolScale * 100);
		Item2_CurrentState->SetText(FText::FromString(FString::Printf(TEXT("Current Reduce -%d%%"), Num)));
		break;
	}
	case 2:
	{
		Num = FMath::RoundToInt(DataTmp->RangeBonus * 100) - 100;
		Item2_CurrentState->SetText(FText::FromString(FString::Printf(TEXT("Current Added +%d%%"), Num)));
		break;
	}
	case 3:
	{
		Item2_CurrentState->SetText(FText::FromString(FString::Printf(TEXT("Current Added +%d"), DataTmp->AdditionalProjectile)));
		break;
	}
	default:
		break;
	}

	FString* ItemName3 = ItemDictionary.Find(Choices[2].Key);
	Item3_Name->SetText(FText::FromString(*ItemName3));
	FString* ItemDescrib3 = EnhanceCode.Find(Choices[2].Value);
	Item3_Des->SetText(FText::FromString(*ItemDescrib3));

	DataTmp = GameInstance->GetUnitBuffData(Choices[2].Key);
	switch (Choices[2].Value)
	{
	case 0:
	{
		Num = FMath::RoundToInt(DataTmp->AttackScale * 100) - 100;
		Item3_CurrentState->SetText(FText::FromString(FString::Printf(TEXT("Current Added +%d%%"), Num)));
		break;
	}
	case 1:
	{
		Num = 100 - FMath::RoundToInt(DataTmp->AttackCoolScale * 100);
		Item3_CurrentState->SetText(FText::FromString(FString::Printf(TEXT("Current Reduce -%d%%"), Num)));
		break;
	}
	case 2:
	{
		Num = FMath::RoundToInt(DataTmp->RangeBonus * 100) - 100;
		Item3_CurrentState->SetText(FText::FromString(FString::Printf(TEXT("Current Added +%d%%"), Num)));
		break;
	}
	case 3:
	{
		Item3_CurrentState->SetText(FText::FromString(FString::Printf(TEXT("Current Added +%d"), DataTmp->AdditionalProjectile)));
		break;
	}
	default:
		break;
	}
}

void UGCRandomBuffSelect::SetGameState()
{
	AGCGameState* GameState = Cast<AGCGameState>(UGameplayStatics::GetGameState(GetWorld()));
	GameState->OnLevelChanged.AddLambda([this]() {
		this->Roll();
		});
}

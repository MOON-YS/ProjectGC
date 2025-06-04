// Fill out your copyright notice in the Description page of Project Settings.


#include "GCGameInstance.h"

UGCGameInstance::UGCGameInstance()
{

	ItemDictionary.Add(EUnitType::CIWS, TEXT("CIWS"));
	ItemDictionary.Add(EUnitType::M1A1, TEXT("M1A1"));
	ItemDictionary.Add(EUnitType::M777, TEXT("M777"));
	ItemDictionary.Add(EUnitType::SEARAM, TEXT("RIM-116"));
	ItemDictionary.Add(EUnitType::MARLIN, TEXT("Marlin"));
	ItemDictionary.Add(EUnitType::GATECLOSER, TEXT("CIWS"));
	ItemDictionary.Add(EUnitType::PATRIOT, TEXT("Patriot"));

	FString UnitBuffDataPath = TEXT("/Script/Engine.DataTable'/Game/Data/UnitBuffData.UnitBuffData'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_GCBUFFDATA(*UnitBuffDataPath);
	GCCHECK(DT_GCBUFFDATA.Succeeded());
	CurrentGameData = DT_GCBUFFDATA.Object;
	
	GCCHECK(CurrentGameData->GetRowMap().Num() > 0);
}

void UGCGameInstance::Init()
{
	Super::Init();
}

void UGCGameInstance::ResetData()
{
	for (TPair<EUnitType, FString>& Pair : ItemDictionary) 
	{
		FGCUnitBuffData* Data = CurrentGameData->FindRow<FGCUnitBuffData>(*Pair.Value, TEXT(""));
		Data->AdditionalProjectile = 0;
		Data->AttackCoolScale = 1.0f;
		Data->AttackScale = 1.0f;
		Data->RangeBonus = 1.0f;
	}
}

FGCUnitBuffData* UGCGameInstance::GetUnitBuffData(EUnitType UnitType)
{
	FString* ItemName = ItemDictionary.Find(UnitType);
	return CurrentGameData->FindRow<FGCUnitBuffData>(**ItemName, TEXT(""));
}

void UGCGameInstance::AddAttackCoolScale(EUnitType UnitType, float Amount)
{
	FString* ItemName = ItemDictionary.Find(UnitType);
	CurrentGameData->FindRow<FGCUnitBuffData>(**ItemName, TEXT(""))->AttackCoolScale -= Amount;
	OnStatChanged.Broadcast();
	switch (UnitType) 
	{
	case EUnitType::M1A1:
	{
		OnCoolChangedM1A1.Broadcast();
	}
	case EUnitType::PATRIOT:
	{
		OnCoolChangedPatriot.Broadcast();
	}
	case EUnitType::SEARAM:
	{
		OnCoolChangedRAM.Broadcast();
	}
	case EUnitType::M777:
	{
		OnCoolChangedM777.Broadcast();
	}
	}
}

void UGCGameInstance::AddAttackScale(EUnitType UnitType, float Amount)
{
	FString* ItemName = ItemDictionary.Find(UnitType);
	CurrentGameData->FindRow<FGCUnitBuffData>(**ItemName, TEXT(""))->AttackScale += Amount;
	OnStatChanged.Broadcast();
}

void UGCGameInstance::AddRangeScale(EUnitType UnitType, float Amount)
{
	FString* ItemName = ItemDictionary.Find(UnitType);
	CurrentGameData->FindRow<FGCUnitBuffData>(**ItemName, TEXT(""))->RangeBonus += Amount;
	OnStatChanged.Broadcast();
}

void UGCGameInstance::AddProjectile(EUnitType UnitType)
{
	FString* ItemName = ItemDictionary.Find(UnitType);
	CurrentGameData->FindRow<FGCUnitBuffData>(**ItemName, TEXT(""))->AdditionalProjectile++;
	OnStatChanged.Broadcast();
}
	
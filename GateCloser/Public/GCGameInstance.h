// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GateCloser.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "GCGameInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnStatChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnCoolChangedM1A1Delegate);
DECLARE_MULTICAST_DELEGATE(FOnCoolChangedPatriotDelegate);
DECLARE_MULTICAST_DELEGATE(FOnCoolChangedRAMDelegate);
DECLARE_MULTICAST_DELEGATE(FOnCoolChangedM777Delegate);

/**
 * 
 */
USTRUCT(BlueprintType)
struct FGCUnitBuffData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FGCUnitBuffData() : UnitType(EUnitType::NONE), AttackScale(1.0f), AttackCoolScale(1.0f), RangeBonus(0.f), AdditionalProjectile(0) {}

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="BuffData")
	EUnitType UnitType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BuffData")
	float AttackScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BuffData")
	float AttackCoolScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BuffData")
	float RangeBonus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BuffData")
	int32 AdditionalProjectile;

};

UCLASS()
class GATECLOSER_API UGCGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UGCGameInstance();
	virtual void Init() override;
	void ResetData();
	FGCUnitBuffData* GetUnitBuffData(EUnitType UnitType);

	void AddAttackCoolScale(EUnitType UnitType, float Amount);
	void AddAttackScale(EUnitType UnitType, float Amount);
	void AddRangeScale(EUnitType UnitType, float Amount);
	void AddProjectile(EUnitType UnitType);

	FOnStatChangedDelegate OnStatChanged;
	FOnCoolChangedM1A1Delegate OnCoolChangedM1A1;
	FOnCoolChangedPatriotDelegate OnCoolChangedPatriot;
	FOnCoolChangedRAMDelegate OnCoolChangedRAM;
	FOnCoolChangedM777Delegate OnCoolChangedM777;

private:
	TMap<EUnitType, FString> ItemDictionary;

	class UDataTable* CurrentGameData;


};

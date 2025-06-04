// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GateCloser.h"
#include "GameFramework/GameStateBase.h"
#include "GCGameState.generated.h"


DECLARE_MULTICAST_DELEGATE(FTimeRemainDelegate);
DECLARE_MULTICAST_DELEGATE(FHPChanageDelegate);
DECLARE_MULTICAST_DELEGATE(FExpChanageDelegate);
DECLARE_MULTICAST_DELEGATE(FLevelChanageDelegate);
DECLARE_MULTICAST_DELEGATE(FCreditChangeDelegate);
/**
 * 
 */
UCLASS()
class GATECLOSER_API AGCGameState : public AGameStateBase
{
	GENERATED_BODY()

private:
	float CurrentExp;
	float MaxExpCurrentLevel;
	float CurrentHp;
	float MaxHp;
	int32 Level;
	int32 Credit;
	int32 CurrentPrice;
	int32 TimeRemain;
	int32 KillScore;
	FTimerHandle TimeHandle;
	float EnemySpeedScale;
	int32 EnemySpawnScale;
	UPROPERTY()
	class UGCUIResult* ResultUI;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UGCUIResult> ResultUIClass;

public:
	AGCGameState();
	void SetGameEnd();
	int32 GetCurrentLevel() { return Level; };
	int32 GetCurrentCredit() { return Credit; };
	int32 GetTimeRemain() { return TimeRemain; };
	float GetCurrentExpRatio() { return (CurrentExp / MaxExpCurrentLevel); };
	float GetCurrentHpRatio() { return (CurrentHp / MaxHp); };
	int32 GetCurrentPrice() { return CurrentPrice; };
	int32 GetCurrentScore() { return KillScore; };
	int32 GetEnemySpawnScale() { return EnemySpawnScale; };
	void SetCurrentHp(float NewHp);
	void SetMaxHp(float NewMaxHp) { MaxHp = NewMaxHp; };
	void AddExp(int32 AddedExp);
	float GetEnemySpeedScale() { return EnemySpeedScale; };

	void LevelUp();
	bool IsPossibleToBuy();
	void AddCredit(int32 AddedCredit);
	void Purchase();
	void TimeFolw();
	virtual void BeginPlay() override;

	FTimeRemainDelegate OnTimeFlow;
	FHPChanageDelegate OnHpChanged;
	FExpChanageDelegate OnExpChanged;
	FLevelChanageDelegate OnLevelChanged;
	FCreditChangeDelegate OnCreditChanged;

};

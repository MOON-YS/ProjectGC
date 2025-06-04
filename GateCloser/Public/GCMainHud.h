// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GateCloser.h"
#include "Blueprint/UserWidget.h"
#include "GCMainHud.generated.h"

/**
 * 
 */
UCLASS()
class GATECLOSER_API UGCMainHud : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	void UpdateTime();
	void UpdateHp();
	void UpdateState();
	void BindGameState();

private:
	UPROPERTY()
	class UProgressBar* HPProgressBar;

	UPROPERTY()
	class UProgressBar* ExpProgressBar;

	UPROPERTY()
	class UTextBlock* TimeCounter;

	UPROPERTY()
	class UTextBlock* Level;

	UPROPERTY()
	class UTextBlock* Credit;

	UPROPERTY()
	class UTextBlock* KillScore;

	class AGCGameState* GameState;
};

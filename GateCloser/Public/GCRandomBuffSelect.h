// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GateCloser.h"
#include "Blueprint/UserWidget.h"
#include "GCRandomBuffSelect.generated.h"

/**
 * 
 */
UCLASS()
class GATECLOSER_API UGCRandomBuffSelect : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnButton1Clicked();

	UFUNCTION()
	void OnButton2Clicked();

	UFUNCTION()
	void OnButton3Clicked();

	void Roll();

	void SetGameState();
private:
	UPROPERTY()
	class UButton* Item1;

	UPROPERTY()
	class UTextBlock* Item1_Name;

	UPROPERTY()
	class UTextBlock* Item1_Des;

	UPROPERTY()
	class UTextBlock* Item1_CurrentState;

	UPROPERTY()
	class UButton* Item2;

	UPROPERTY()
	class UTextBlock* Item2_Name;

	UPROPERTY()
	class UTextBlock* Item2_Des;

	UPROPERTY()
	class UTextBlock* Item2_CurrentState;

	UPROPERTY()
	class UButton* Item3;

	UPROPERTY()
	class UTextBlock* Item3_Name;

	UPROPERTY()
	class UTextBlock* Item3_Des;

	UPROPERTY()
	class UTextBlock* Item3_CurrentState;

	TArray<TPair<EUnitType, int32>> Choices;


	TMap<EUnitType, FString> ItemDictionary;
	TMap<int32, FString> EnhanceCode;
	

};

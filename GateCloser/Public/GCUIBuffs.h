// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GateCloser.h"
#include "Blueprint/UserWidget.h"
#include "GCUIBuffs.generated.h"

/**
 * 
 */
UCLASS()
class GATECLOSER_API UGCUIBuffs : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void Update();

	UFUNCTION()
	void OnCloseButtonClicked();
private:

	TArray<TPair<EUnitType, FString>> List;

	UPROPERTY()
	class UButton* Close;

	UPROPERTY()
	class UTextBlock* GC_AD;

	UPROPERTY()
	class UTextBlock* GC_ACR;

	UPROPERTY()
	class UTextBlock* GC_AR;

	UPROPERTY()
	class UTextBlock* GC_AP;



	UPROPERTY()
	class UTextBlock* M1A1_AD;

	UPROPERTY()
	class UTextBlock* M1A1_ACR;

	UPROPERTY()
	class UTextBlock* M1A1_AR;

	UPROPERTY()
	class UTextBlock* M1A1_AP;



	UPROPERTY()
	class UTextBlock* Marlin_AD;

	UPROPERTY()
	class UTextBlock* Marlin_ACR;

	UPROPERTY()
	class UTextBlock* Marlin_AR;

	UPROPERTY()
	class UTextBlock* Marlin_AP;



	UPROPERTY()
	class UTextBlock* CIWS_AD;

	UPROPERTY()
	class UTextBlock* CIWS_ACR;

	UPROPERTY()
	class UTextBlock* CIWS_AR;

	UPROPERTY()
	class UTextBlock* CIWS_AP;



	UPROPERTY()
	class UTextBlock* RAM_AD;

	UPROPERTY()
	class UTextBlock* RAM_ACR;

	UPROPERTY()
	class UTextBlock* RAM_AR;

	UPROPERTY()
	class UTextBlock* RAM_AP;



	UPROPERTY()
	class UTextBlock* Patriot_AD;

	UPROPERTY()
	class UTextBlock* Patriot_ACR;

	UPROPERTY()
	class UTextBlock* Patriot_AR;

	UPROPERTY()
	class UTextBlock* Patriot_AP;



	UPROPERTY()
	class UTextBlock* M777_AD;

	UPROPERTY()
	class UTextBlock* M777_ACR;

	UPROPERTY()
	class UTextBlock* M777_AR;

	UPROPERTY()
	class UTextBlock* M777_AP;

};

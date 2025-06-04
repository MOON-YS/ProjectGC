// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GateCloser.h"
#include "Blueprint/UserWidget.h"
#include "GCSelectModeWidget.generated.h"

/**
 * 
 */
UCLASS()
class GATECLOSER_API UGCSelectModeWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void BindPlayerState(AGCPlayerState* InputPlayerState);

	UFUNCTION()
	void BindPlayerController(class AGCPlayerController* InputPlayerController);

	void BindBuffsWidget(class UGCUIBuffs* NewBuffs) { Buffs = NewBuffs; };

protected:

	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnSpawnModeButtonClicked();

	UFUNCTION()
	void OnViewModeButtonClicked();

	UFUNCTION()
	void OnStatButtonClicked();

private:
	UPROPERTY()
	class UButton* SpawnModeButton;

	UPROPERTY()
	class UButton* ViewModeButton;

	UPROPERTY()
	class UButton* StatButton;

	UPROPERTY()
	class AGCPlayerState* GCPlayerState;

	UPROPERTY()
	class AGCPlayerController* GCPlayerController;

	class UGCUIBuffs* Buffs;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "GCSelectModeWidget.h"
#include "GCPlayerState.h"
#include "GCPlayerController.h"
#include "Components/Button.h"
#include "GCUIBuffs.h"
void UGCSelectModeWidget::BindPlayerState(AGCPlayerState* InputPlayerState)
{
	GCPlayerState = InputPlayerState;
}
void UGCSelectModeWidget::BindPlayerController(AGCPlayerController* InputPlayerController)
{
	GCPlayerController = InputPlayerController;
}

void UGCSelectModeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	SpawnModeButton = Cast<UButton>(GetWidgetFromName(TEXT("btnSpawn")));
	if (SpawnModeButton != nullptr)
	{
		SpawnModeButton->OnReleased.AddDynamic(this, &UGCSelectModeWidget::OnSpawnModeButtonClicked);
	}
	ViewModeButton = Cast<UButton>(GetWidgetFromName(TEXT("btnView")));
	if (ViewModeButton != nullptr)
	{
		ViewModeButton->OnReleased.AddDynamic(this, &UGCSelectModeWidget::OnViewModeButtonClicked);
	}
	StatButton = Cast<UButton>(GetWidgetFromName(TEXT("btnStat")));
	if (StatButton != nullptr)
	{
		StatButton->OnReleased.AddDynamic(this, &UGCSelectModeWidget::OnStatButtonClicked);
	}
	bIsFocusable = true;
	//SetVisibility(ESlateVisibility::Hidden);

	//OnKeyUp()
}

void UGCSelectModeWidget::OnSpawnModeButtonClicked()
{
	GCPlayerState->SetPlayerCurrentControlMode(EPlayerControlMode::BUILD);
	SetVisibility(ESlateVisibility::Hidden);

	FInputModeUIOnly InputMode;
	GCPlayerController->SetInputMode(InputMode);
}	

void UGCSelectModeWidget::OnViewModeButtonClicked()
{
	GCPlayerState->SetPlayerCurrentControlMode(EPlayerControlMode::VIEW);
	SetVisibility(ESlateVisibility::Hidden);
	FInputModeGameOnly InputMode;
	GCPlayerController->SetInputMode(InputMode);
}

void UGCSelectModeWidget::OnStatButtonClicked()
{
	GCPlayerState->SetPlayerCurrentControlMode(EPlayerControlMode::VIEW);
	if (Buffs != nullptr)
	{
		Buffs->Update();
		Buffs->SetVisibility(ESlateVisibility::Visible);
	}
	SetVisibility(ESlateVisibility::Hidden);
	FInputModeUIOnly InputMode;
	GCPlayerController->SetInputMode(InputMode);
}


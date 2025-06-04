// Fill out your copyright notice in the Description page of Project Settings.


#include "GCSpawnHUDWidget.h"
#include "GCPlayerController.h"
#include "GCGameActorManager.h"
#include "Components/Button.h"
#include "GCPlayerState.h"
void UGCSpawnHUDWidget::BindPlayerState(AGCPlayerState* PlayerState)
{
	GCPlayerState = PlayerState;
	PlayerState->OnControlModeChanged.AddUObject(this, &UGCSpawnHUDWidget::ToggleUIVisible);
}
void UGCSpawnHUDWidget::BindPlayerController(AGCPlayerController* PlayerController)
{
	GCPlayerController = PlayerController;
}
void UGCSpawnHUDWidget::BindActorManager(AGCGameActorManager* NewActorManager)
{
	ActorManager = NewActorManager;
}
void UGCSpawnHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SpawnM777Button = Cast<UButton>(GetWidgetFromName(TEXT("btnSpawnM777")));
	if (SpawnM777Button != nullptr) 
	{
		SpawnM777Button->OnClicked.AddDynamic(this, &UGCSpawnHUDWidget::OnSpawnM777Clicked);
	}
	SpawnPatriotButton = Cast<UButton>(GetWidgetFromName(TEXT("btnSpawnPatriot")));
	if (SpawnPatriotButton != nullptr)
	{
		SpawnPatriotButton->OnClicked.AddDynamic(this, &UGCSpawnHUDWidget::OnSpawnPatriotClicked);
	}
	SpawnM1A1Button = Cast<UButton>(GetWidgetFromName(TEXT("btnSpawnM1A1")));
	if (SpawnM1A1Button != nullptr)
	{
		SpawnM1A1Button->OnClicked.AddDynamic(this, &UGCSpawnHUDWidget::OnSpawnM1A1Clicked);
	}
	SpawnCIWSButton = Cast<UButton>(GetWidgetFromName(TEXT("btnSpawnCIWS")));
	if (SpawnCIWSButton != nullptr)
	{
		SpawnCIWSButton->OnClicked.AddDynamic(this, &UGCSpawnHUDWidget::OnSpawnCIWSClicked);
	}
	SpawnRAMButton = Cast<UButton>(GetWidgetFromName(TEXT("btnSpawnRAM")));
	if (SpawnCIWSButton != nullptr)
	{
		SpawnRAMButton->OnClicked.AddDynamic(this, &UGCSpawnHUDWidget::OnSpawnRAMClicked);
	}
	SpawnMarlinButton = Cast<UButton>(GetWidgetFromName(TEXT("btnSpawnMarlin")));
	if (SpawnMarlinButton != nullptr)
	{
		SpawnMarlinButton->OnClicked.AddDynamic(this, &UGCSpawnHUDWidget::OnSpawnMarlinClicked);
	}
	CloseButton = Cast<UButton>(GetWidgetFromName(TEXT("btnClose")));
	if (CloseButton != nullptr)
	{
		CloseButton->OnClicked.AddDynamic(this, &UGCSpawnHUDWidget::OnCloseClicked);
	}
	SetVisibility(ESlateVisibility::Hidden);
}


void UGCSpawnHUDWidget::OnSpawnM777Clicked()
{
	GCCHECK(GCPlayerController != nullptr);
	GCPlayerController->SwapBuildTypeOnCursor(EUnitType::M777);
	FInputModeGameAndUI InputMode;
	GCPlayerController->SetInputMode(InputMode);
	GCPlayerController->bShowMouseCursor = false;
	SetCursor(EMouseCursor::Crosshairs);
	SetVisibility(ESlateVisibility::Hidden);

}

void UGCSpawnHUDWidget::OnSpawnPatriotClicked()
{
	GCCHECK(GCPlayerController != nullptr);
	GCPlayerController->SwapBuildTypeOnCursor(EUnitType::PATRIOT);
	FInputModeGameAndUI InputMode;
	GCPlayerController->SetInputMode(InputMode);
	GCPlayerController->bShowMouseCursor = false;
	SetCursor(EMouseCursor::Crosshairs);
	SetVisibility(ESlateVisibility::Hidden);

}

void UGCSpawnHUDWidget::OnSpawnM1A1Clicked()
{
	GCCHECK(GCPlayerController != nullptr);
	GCPlayerController->SwapBuildTypeOnCursor(EUnitType::M1A1);
	FInputModeGameAndUI InputMode;
	GCPlayerController->SetInputMode(InputMode);
	GCPlayerController->bShowMouseCursor = false;
	SetCursor(EMouseCursor::Crosshairs);
	SetVisibility(ESlateVisibility::Hidden);

}

void UGCSpawnHUDWidget::OnSpawnCIWSClicked()
{
	GCCHECK(GCPlayerController != nullptr);
	GCPlayerController->SwapBuildTypeOnCursor(EUnitType::CIWS);
	FInputModeGameAndUI InputMode;
	GCPlayerController->SetInputMode(InputMode);
	GCPlayerController->bShowMouseCursor = false;
	SetCursor(EMouseCursor::Crosshairs);
	SetVisibility(ESlateVisibility::Hidden);

}

void UGCSpawnHUDWidget::OnSpawnRAMClicked()
{
	GCCHECK(GCPlayerController != nullptr);
	GCPlayerController->SwapBuildTypeOnCursor(EUnitType::SEARAM);
	FInputModeGameAndUI InputMode;
	GCPlayerController->SetInputMode(InputMode);
	GCPlayerController->bShowMouseCursor = false;
	SetCursor(EMouseCursor::Crosshairs);
	SetVisibility(ESlateVisibility::Hidden);
}

void UGCSpawnHUDWidget::OnSpawnMarlinClicked()
{
	GCCHECK(GCPlayerController != nullptr);
	GCPlayerController->SwapBuildTypeOnCursor(EUnitType::MARLIN);
	FInputModeGameAndUI InputMode;
	GCPlayerController->SetInputMode(InputMode);
	GCPlayerController->bShowMouseCursor = false;
	SetCursor(EMouseCursor::Crosshairs);
	SetVisibility(ESlateVisibility::Hidden);

}

void UGCSpawnHUDWidget::OnCloseClicked()
{
	GCPlayerState->SetPlayerCurrentControlMode(EPlayerControlMode::VIEW);
	SetVisibility(ESlateVisibility::Hidden);
	FInputModeGameOnly InputMode;
	GCPlayerController->SetInputMode(InputMode);
	SetCursor(EMouseCursor::Default);
}

void UGCSpawnHUDWidget::ToggleUIVisible()
{
	if(GCPlayerState->GetPlayerCurrentControlMode() == EPlayerControlMode::BUILD) SetVisibility(ESlateVisibility::Visible);
	else SetVisibility(ESlateVisibility::Hidden);
}


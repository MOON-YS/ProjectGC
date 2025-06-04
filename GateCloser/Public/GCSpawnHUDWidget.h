// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GateCloser.h"
#include "Blueprint/UserWidget.h"
#include "GCPlayerController.h"
#include "GCPlayerController.h"
#include "GCSpawnHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class GATECLOSER_API UGCSpawnHUDWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void BindPlayerState(class AGCPlayerState* PlayerState);
	void BindPlayerController(class AGCPlayerController* PlayerController);
	void BindActorManager(class AGCGameActorManager* NewActorManager);
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnSpawnM777Clicked();

	UFUNCTION()
	void OnSpawnPatriotClicked();

	UFUNCTION()
	void OnSpawnM1A1Clicked();

	UFUNCTION()
	void OnSpawnCIWSClicked();

	UFUNCTION()
	void OnSpawnRAMClicked();

	UFUNCTION()
	void OnSpawnMarlinClicked();

	UFUNCTION()
	void OnCloseClicked();

private:
	UFUNCTION()
	void ToggleUIVisible();

private:
	UPROPERTY()
	class UButton* SpawnM777Button;

	UPROPERTY()
	class UButton* SpawnPatriotButton;

	UPROPERTY()
	class UButton* SpawnM1A1Button;

	UPROPERTY()
	class UButton* SpawnCIWSButton;

	UPROPERTY()
	class UButton* SpawnRAMButton;

	UPROPERTY()
	class UButton* SpawnMarlinButton;

	UPROPERTY()
	class UButton* CloseButton;

	UPROPERTY()
	class AGCPlayerState* GCPlayerState;

	UPROPERTY()
	class AGCPlayerController* GCPlayerController;

	UPROPERTY()
	class AGCGameActorManager* ActorManager;
};

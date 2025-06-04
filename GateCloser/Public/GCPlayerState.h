// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GateCloser.h"
#include "GameFramework/PlayerState.h"
#include "GCPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnControlModeChangedDelegate);

UCLASS()
class GATECLOSER_API AGCPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	UFUNCTION()
	EPlayerControlMode GetPlayerCurrentControlMode();

	UFUNCTION()
	void SetPlayerCurrentControlMode(EPlayerControlMode NewMode);

	FOnControlModeChangedDelegate OnControlModeChanged;

private:
	UPROPERTY(VisibleInstanceOnly, Category = "InputMode")
	EPlayerControlMode CurrentControlMode = EPlayerControlMode::VIEW;

};

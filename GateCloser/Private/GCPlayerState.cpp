// Fill out your copyright notice in the Description page of Project Settings.


#include "GCPlayerState.h"

EPlayerControlMode AGCPlayerState::GetPlayerCurrentControlMode()
{
	return CurrentControlMode;
}

void AGCPlayerState::SetPlayerCurrentControlMode(EPlayerControlMode NewMode)
{
	CurrentControlMode = NewMode;
	OnControlModeChanged.Broadcast();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "GCGameMode.h"
#include "GCPlayerController.h"
#include "GCPlayer.h"
#include "GCSpectatorPawn.h"
#include "GCPlayerState.h"
#include "GCGameInstance.h"
#include "GCGameState.h"

AGCGameMode::AGCGameMode()
{
	DefaultPawnClass = AGCPlayer::StaticClass();
	PlayerControllerClass = AGCPlayerController::StaticClass();
	GameStateClass = AGCGameState::StaticClass();
	PlayerStateClass = AGCPlayerState::StaticClass();

}

void AGCGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//GCLOG(Log, TEXT("PostInitlalized"));
}

void AGCGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

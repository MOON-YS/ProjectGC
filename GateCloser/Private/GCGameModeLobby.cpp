// Fill out your copyright notice in the Description page of Project Settings.


#include "GCGameModeLobby.h"
#include "GCMainLobby.h"

AGCGameModeLobby::AGCGameModeLobby()
{
	static ConstructorHelpers::FClassFinder<UGCMainLobby> UI_LOBBY_C(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/UI_MainLobby.UI_MainLobby_C'"));
	if (UI_LOBBY_C.Succeeded())
	{
		MainLobbyUIClass = UI_LOBBY_C.Class;
	}
}

void AGCGameModeLobby::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
	MainLobbyUI = CreateWidget<UGCMainLobby>(GetWorld()->GetFirstPlayerController(), MainLobbyUIClass);
	MainLobbyUI->AddToViewport(0);
	MainLobbyUI->SetVisibility(ESlateVisibility::Visible);	
	FInputModeUIOnly InputMode;
	GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GateCloser.h"
#include "GameFramework/GameMode.h"
#include "GCGameModeLobby.generated.h"

/**
 * 
 */
UCLASS()
class GATECLOSER_API AGCGameModeLobby : public AGameMode
{
	GENERATED_BODY()

public:
	AGCGameModeLobby();

	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UGCMainLobby> MainLobbyUIClass;

private:
	UPROPERTY()
	class UGCMainLobby* MainLobbyUI;
};

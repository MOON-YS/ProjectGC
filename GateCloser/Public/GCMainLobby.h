// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GateCloser.h"
#include "Blueprint/UserWidget.h"
#include "GCMainLobby.generated.h"

/**
 * 
 */
UCLASS()
class GATECLOSER_API UGCMainLobby : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
};

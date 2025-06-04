// Fill out your copyright notice in the Description page of Project Settings.

#include "GateCloser.h"
#include "GameFramework/GameModeBase.h"
#include "GCGameMode.generated.h"

/**
 * 
 */
UCLASS()
class GATECLOSER_API AGCGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AGCGameMode();

	virtual void PostInitializeComponents();
	virtual void PostLogin(APlayerController* NewPlayer) override;
	

};

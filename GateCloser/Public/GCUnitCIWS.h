// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GateCloser.h"
#include "GCUnitBase.h"
#include "GCUnitCIWS.generated.h"

/**
 * 
 */
UCLASS()
class GATECLOSER_API AGCUnitCIWS : public AGCUnitBase
{
	GENERATED_BODY()
	
public:
	AGCUnitCIWS();
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	void SetTimer();
	void DelayDamage();
	
private:
	void Attacking();
	void ToggleSoundPlay();
	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* NiagaraComponentFire;
	FTimerHandle DelayedTimerHandle;
};

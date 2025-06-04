// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GateCloser.h"
#include "GCUnitBase.h"
#include "GCUnitGateCloser.generated.h"

/**
 * 
 */
UCLASS()
class GATECLOSER_API AGCUnitGateCloser : public AGCUnitBase
{
	GENERATED_BODY()
	
public:
	AGCUnitGateCloser();
	void Fire();
	void SetTarget();

	void SetDamage(float Damage);
	void DelayedDamage();
protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
private:
	UPROPERTY(VisibleAnywhere)
	float MaxHP;

	UPROPERTY(VisibleAnywhere)
	float CurrentHP;

	class UNiagaraComponent* NiagaraComponent;
	FTimerHandle RotatingHandler;
	FTimerHandle DelayDamageHandle;
	bool bPreviousRTFState;
	FVector LockedLocation;

	class AGCGameState* GameState;
};

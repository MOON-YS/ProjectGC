// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GateCloser.h"
#include "GCUnitBase.h"
#include "GCUnitRAM.generated.h"

/**
 * 
 */
UCLASS()
class GATECLOSER_API AGCUnitRAM : public AGCUnitBase
{
	GENERATED_BODY()
	
public:
	AGCUnitRAM();
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	void SetTimer();
	void Fire();
	void FireMissiles();
	void SetTarget();
	void ResetTimer();
private:
	int32 EnabledMissileCount = 0;
	int32 MissileFireRepeat = 0;
	FTimerHandle FireMissileTimerHandle;
	FVector CurrentLoockDir;
};

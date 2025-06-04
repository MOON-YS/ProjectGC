// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GateCloser.h"
#include "GCUnitBase.h"
#include "GCUnitMarlin.generated.h"

/**
 * 
 */
UCLASS()
class GATECLOSER_API AGCUnitMarlin : public AGCUnitBase
{
	GENERATED_BODY()

public:
	AGCUnitMarlin();
	virtual void PostInitializeComponents() override;
	void SetTimer();
	void Attacking();
	void Rotating();

private:
	FTimerHandle RotatingHandler;
	bool bPreviousRTFState;
	FVector LockedLocation;
};

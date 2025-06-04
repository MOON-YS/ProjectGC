// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GateCloser.h"
#include "GCUnitBase.h"
#include "GCUnitPatriot.generated.h"

/**
 * 
 */
UCLASS()
class GATECLOSER_API AGCUnitPatriot : public AGCUnitBase
{
	GENERATED_BODY()
	
public:
	AGCUnitPatriot();
	virtual void Tick(float DeltaTime) override;
	void Fire();
	void SetRotate();
	void SetTimer();
	void ResetTimer();
protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
};
	
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GateCloser.h"
#include "GCProjectile.h"
#include "GCProjectileRAM.generated.h"

/**
 * 
 */
UCLASS()
class GATECLOSER_API AGCProjectileRAM : public AGCProjectile
{
	GENERATED_BODY()
	
public:
	AGCProjectileRAM();

protected:
	virtual void BeginPlay() override;
	void Explosion();

public:
	virtual void Tick(float DeltaTime) override;
};

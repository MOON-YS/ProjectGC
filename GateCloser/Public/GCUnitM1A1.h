// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GateCloser.h"
#include "GCUnitBase.h"
#include "GCUnitM1A1.generated.h"

/**
 * 
 */
UCLASS()
class GATECLOSER_API AGCUnitM1A1 : public AGCUnitBase
{
	GENERATED_BODY()

public:
	AGCUnitM1A1();
	void SetTarget();
	void Fire();
	void DelayedFire();
	void SetTimer();
	void ResetTimer();
	
protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	FTimerHandle AttackDelayHandle;
	class AGCEnemy* CurrentTargetEnemy;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Particles", meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* CascadeParticleComponentFire;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Particles", meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* CascadeParticleComponentSmoke;

	FRotator CurrentLoockDir;
	FVector LockedLocation;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GCUnitBase.h"
#include "GCUnitM777.generated.h"

/**
 * 
 */
UCLASS()
class GATECLOSER_API AGCUnitM777 : public AGCUnitBase
{
	GENERATED_BODY()
public:
	AGCUnitM777();
	void Fire();
	void SetRotate();
	void SetTimer();
	void ResetTimer();
	void ResetExplosion();
protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Particles", meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* CascadeParticleComponentFire;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Particles", meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* CascadeParticleComponentSmoke;

	class AGCExplosion* Explosion;

	FVector TargetLocation;
};

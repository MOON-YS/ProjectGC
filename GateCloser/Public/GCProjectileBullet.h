// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GCProjectile.h"
#include "GCProjectileBullet.generated.h"

/**
 * 
 */
UCLASS()
class GATECLOSER_API AGCProjectileBullet : public AGCProjectile
{
	GENERATED_BODY()

public:
	AGCProjectileBullet();
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	FVector TargetLocation;
	
	void SetTargetLocation(FVector NewTargetLocation);
};

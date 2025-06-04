// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GateCloser.h"
#include "Components/ActorComponent.h"
#include "GCUnitBuffComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GATECLOSER_API UGCUnitBuffComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGCUnitBuffComponent();
	void SetTypeOfUnit(EUnitType NewUnitType) { TypeOfUnit = NewUnitType; };
	float GetAttackScaleBonus();
	float GetAttackCoolScaleBonus();
	float RangeScaleBonus();
	int32 GetAdditionalProjectile();

	void UpdateStat();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	EUnitType TypeOfUnit;
	float AttackScaleBonus = 1.0;
	float AttackCoolScaleBonus = 1.0;
	float RangeScale = 1.0;
	int32 AdditionalProjectile = 0;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};

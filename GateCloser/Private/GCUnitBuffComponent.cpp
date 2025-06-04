// Fill out your copyright notice in the Description page of Project Settings.


#include "GCUnitBuffComponent.h"
#include "GCGameInstance.h"
// Sets default values for this component's properties
UGCUnitBuffComponent::UGCUnitBuffComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

float UGCUnitBuffComponent::GetAttackScaleBonus()
{
	return AttackScaleBonus;
}
	
float UGCUnitBuffComponent::GetAttackCoolScaleBonus()
{
	return AttackCoolScaleBonus;
}

float UGCUnitBuffComponent::RangeScaleBonus()
{
	return RangeScale;
}

int32 UGCUnitBuffComponent::GetAdditionalProjectile()
{
	return AdditionalProjectile;
}

void UGCUnitBuffComponent::UpdateStat()
{
	auto GameInstance = Cast<UGCGameInstance>(GetWorld()->GetGameInstance());
	FGCUnitBuffData* Data = GameInstance->GetUnitBuffData(TypeOfUnit);

	AttackCoolScaleBonus = Data->AttackCoolScale;
	AttackScaleBonus = Data->AttackScale;
	RangeScale = Data->RangeBonus;
	AdditionalProjectile = Data->AdditionalProjectile;
}


// Called when the game starts
void UGCUnitBuffComponent::BeginPlay()
{
	Super::BeginPlay();
	auto GameInstance = Cast<UGCGameInstance>(GetWorld()->GetGameInstance());
	// ...
	GameInstance->OnStatChanged.AddUObject(this, &UGCUnitBuffComponent::UpdateStat);
	UpdateStat();
}


// Called every frame
void UGCUnitBuffComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


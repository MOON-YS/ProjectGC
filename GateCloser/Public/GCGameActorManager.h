// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GateCloser.h"
#include "GameFramework/Actor.h"
#include "GCGameActorManager.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnActorDeployedDelegate)

UCLASS()
class GATECLOSER_API AGCGameActorManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGCGameActorManager();
	AActor* SpawnAI(FVector SpawnPos,FRotator SpawnRotation, EUnitType AIType);
	void SetActorSpawnBoxDisable(AActor* Actor);
	bool GetActorCanBeDeploy(AActor* Actor, EUnitType InputAItype);
	void TimerTest();
	AActor * GetEnemyRandom();
	AActor * GetEnemyClosest();
	AActor * GetEnemyByMinDistance(FVector FiredLocation,float MinDistance);
	TArray<class AGCEnemy*> GetEnemyByClosestDistance(FVector CurrentLocation, float MaxDistance);
	void SortEnemyByDist();

	FOnActorDeployedDelegate OnActorDeployedDelegate;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	TArray<AActor*> FoundEnemies;
	AActor* SpawnedActor;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UWorld* World;

};

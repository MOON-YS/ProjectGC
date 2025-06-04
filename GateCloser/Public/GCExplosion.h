// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GateCloser.h"
#include "GameFramework/Actor.h"
#include "GCExplosion.generated.h"

UCLASS()
class GATECLOSER_API AGCExplosion : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGCExplosion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	

	
public:
	void PlayExplosion();
	void PlayExplosionStart();
	void BindActorManager(class AGCGameActorManager* NewActorManager);
	void SetDamage(float NewDamage) { Damage = NewDamage; };
	void SetExplosionRange(float NewRange) { ExplosionRange = NewRange; };
	float GetExplosionRange() { return ExplosionRange; };
private:
	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* NiagaraComponentExp;

	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* NiagaraComponentAOE;

	UPROPERTY(VisibleAnywhere)
	float ExplosionRange;

	UPROPERTY(VisibleAnywhere)
	float ExplosionDelay;

	UPROPERTY(VisibleAnywhere)
	float Damage;

	FTimerHandle TimerHandleExplosion;
	FTimerHandle TimerHandleAOE;

	TArray<class AGCEnemy*> Enemies;

	class AGCGameActorManager* ActorManager;
};

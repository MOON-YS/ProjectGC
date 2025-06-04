// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GateCloser.h"
#include "GameFramework/Actor.h"
#include "GCProjectile.generated.h"

UCLASS()
class GATECLOSER_API AGCProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGCProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetTarget(TWeakObjectPtr<class AGCEnemy> NewTarget);
	void BindActorMananger(class AGCGameActorManager* NewActorManager);
	void SetDamage(float NewDamage) { Damage = NewDamage; };

protected:
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Mesh;
	
	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* Capsule;
	
	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* NiagaraComponentTrail;
	
	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* NiagaraComponentHit;
	float Damage;
	float Speed;
	TWeakObjectPtr<class AGCEnemy> Target = nullptr;
	TWeakObjectPtr<class AGCEnemy> LockedTarget = nullptr;
	class AGCGameActorManager* ActorManager;
	FRotator NewRotation;
	FRotator LookAtRotation;
	FRotator CurrentRotation;
	FVector TargetVector;
	bool bLocked;
	float EstimatedTime = 0.0;
	float TargetDist;
	FVector LockedLocation;
	FTimerHandle TimerHandleAfterLaunch;
	FTimerHandle TimerHandleExplosion;
};

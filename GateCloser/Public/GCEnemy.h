// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GateCloser.h"
#include "GameFramework/Actor.h"
#include "GCEnemy.generated.h"
DECLARE_MULTICAST_DELEGATE(OnHPChangedDelegate);

UCLASS()
class GATECLOSER_API AGCEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGCEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherbodyIndex, bool bFromSwepp, const FHitResult& SweepResult);

public:	
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void BindActorManager(class AGCGameActorManager* NewActorManager);
	float GetHPRatio();
	void SetDamage(float DamageAmount);
	FVector GetDirection();
	float GetSpeed();
	OnHPChangedDelegate OnHPChanged;

	class UNiagaraSystem* NiagaraSystem;
private:
	UStaticMesh* StaticMesh_01;
	UStaticMesh* StaticMesh_02;
	USkeletalMesh* SkeletalMesh_0;
	UStaticMeshComponent* StaticMeshComponent;
	USkeletalMeshComponent* Mesh;
	class AGCGameActorManager* ActorManager;
	class UBoxComponent* Box;
	class UWidgetComponent* HPBarWidget;

	float ZAxis;
	float MaxHP;
	float CurrentHP;
	float Speed;
	
	bool bAlive;
	FVector MoveDirectionNormal;
	FTimerHandle DeadTimerHandle;

	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GateCloser.h"
#include "GameFramework/Pawn.h"
#include "GCUnitBase.generated.h"

UCLASS()
class GATECLOSER_API AGCUnitBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGCUnitBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherbodyIndex, bool bFromSwepp, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherbodyIndex);

	virtual void PostInitializeComponents() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	bool GetCanBeDeploy();

	UFUNCTION()
	void SetCanAttack(bool NewBooleen);
	
	UFUNCTION()
	FRotator GetTargetRotate(TWeakObjectPtr<AGCEnemy> NewTargetActor);

	UFUNCTION()
	void BindActorManager(class AGCGameActorManager* NewActorManager);

	UFUNCTION()
	void SetAnimDeployed();

	void ToggleSpawnMesh();

protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Unit STAT", Meta = (AllowPrivateAccess = true))
	EUnitType Type = EUnitType::NONE;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Unit STAT", Meta = (AllowPrivateAccess = true))
	float AttackCool;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Unit STAT", Meta = (AllowPrivateAccess = true))
	float AttackCoolScale;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Unit STAT", Meta = (AllowPrivateAccess = true))
	float AttackPoint;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Unit STAT", Meta = (AllowPrivateAccess = true))
	float AttackPointScale;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "AI STAT", Meta = (AllowPrivateAccess = true))
	bool bCanAttack;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "AI STAT", Meta = (AllowPrivateAccess = true))
	bool bCanBeDeploy;

	UPROPERTY(VisibleAnywhere, Category = Collision)
	UBoxComponent* Box;

	UPROPERTY(VisibleAnywhere, Category = Visual)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, Category = Visual)
	UStaticMeshComponent* SpawnMesh;

	UPROPERTY(VisibleAnywhere, Category = Visual)
	UMaterialInstance* EmptyMaterial;

	UPROPERTY(VisibleAnywhere, Category = Visual)
	UMaterialInstance* SpawnAbleMaterial;

	UPROPERTY(VisibleAnywhere, Category = Visual)
	UMaterialInstance* SpawnEnableMaterial;

	UPROPERTY(VisibleAnywhere, Category = "ActorManager")
	AGCGameActorManager* ActorManager;

	UPROPERTY(VisibleAnywhere)
	class UGCUnitBuffComponent* BuffComponent;

	UPROPERTY(VisibleAnywhere, Category = "Sound")
	class USoundCue* SoundCue;

	UPROPERTY()
	UAudioComponent* AudioComponent;

	TWeakObjectPtr<class AGCEnemy> CurrentTargetActor;
	FTimerHandle AttackTimerHandler;
	class UGCAllyAnimInstance* GCAnimInstance;
	FVector PitchPos = FVector::ZeroVector;
	class AGCGameState* GameState;

};

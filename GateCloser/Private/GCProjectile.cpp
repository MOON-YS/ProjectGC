// Fill out your copyright notice in the Description page of Project Settings.


#include "GCProjectile.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GCEnemy.h"
// Sets default values
AGCProjectile::AGCProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULE"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MESH"));

	RootComponent = Mesh;
	Capsule->SetupAttachment(Mesh);
	Capsule->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));

	NiagaraComponentTrail = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponentTrail->SetupAttachment(RootComponent);

	NiagaraComponentHit = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponentExplosion"));
	NiagaraComponentHit->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AGCProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGCProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGCProjectile::SetTarget(TWeakObjectPtr<AGCEnemy> NewTarget)
{
	Target = NewTarget;
}

void AGCProjectile::BindActorMananger(AGCGameActorManager* NewActorManager)
{
	ActorManager = NewActorManager;
}


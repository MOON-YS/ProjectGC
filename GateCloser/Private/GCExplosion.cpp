// Fill out your copyright notice in the Description page of Project Settings.


#include "GCExplosion.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GCEnemy.h"
#include "GCGameActorManager.h"
// Sets default values
AGCExplosion::AGCExplosion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	NiagaraComponentExp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Explosion"));
	NiagaraComponentAOE = CreateDefaultSubobject<UNiagaraComponent>(TEXT("AOE"));
	NiagaraComponentAOE->bNeverDistanceCull = true;
	NiagaraComponentAOE->bAllowCullDistanceVolume = false;
	RootComponent = NiagaraComponentAOE;
	NiagaraComponentExp->SetupAttachment(RootComponent);
	NiagaraComponentExp->SetRelativeLocation(FVector(0.f, 0.f, 1.0f));
	//NiagaraComponentAOE->SetRelativeLocation(FVector(0.f, 0.f, 30.0f));
	NiagaraComponentExp->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f));
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraSystemAsset(TEXT("/Script/Niagara.NiagaraSystem'/Game/VFX/NS_Explosion_Sand.NS_Explosion_Sand'"));
	if (NiagaraSystemAsset.Succeeded())
	{
		NiagaraComponentExp->SetAsset(NiagaraSystemAsset.Object);
		NiagaraComponentExp->bAutoActivate = false; // 컴포넌트가 자동 실행되도록 설정
	}
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraSystemAssetAOE(TEXT("/Script/Niagara.NiagaraSystem'/Game/MapTrackMarkers/Effects/P_Marker_13.P_Marker_13'"));
	if (NiagaraSystemAsset.Succeeded())
	{
		NiagaraComponentAOE->SetAsset(NiagaraSystemAssetAOE.Object);
		NiagaraComponentAOE->bAutoActivate = false; // 컴포넌트가 자동 실행되도록 설정
	}
	ExplosionRange = 2500;

}

// Called when the game starts or when spawned
void AGCExplosion::BeginPlay()
{
	Super::BeginPlay();
	NiagaraComponentAOE->SetVariableFloat(TEXT("LifeTime"), 3);
	NiagaraComponentAOE->SetVariableFloat(TEXT("CircleSize"), ExplosionRange);
}

// Called every frame
void AGCExplosion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGCExplosion::PlayExplosion()
{

	NiagaraComponentExp->SetActive(true);
	Enemies = ActorManager->GetEnemyByClosestDistance(GetActorLocation(), ExplosionRange/2);
	for (AGCEnemy* TmpEnemy : Enemies) 
	{
		if (IsValid(TmpEnemy))
		{
			TmpEnemy->SetDamage(Damage);
		}
	}
}

void AGCExplosion::PlayExplosionStart()
{

	NiagaraComponentAOE->SetActive(true);
	GetWorld()->GetTimerManager().SetTimer(TimerHandleExplosion, this, &AGCExplosion::PlayExplosion, 3.0f, false);
}

void AGCExplosion::BindActorManager(AGCGameActorManager* NewActorManager)
{
	ActorManager = NewActorManager;
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "GCProjectileBullet.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GCEnemy.h"
#include "Kismet/KismetMathLibrary.h"
#include "GCGameActorManager.h"

AGCProjectileBullet::AGCProjectileBullet()
{
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraSystemAssetExp(TEXT("/Script/Niagara.NiagaraSystem'/Game/VFX/NS_bullet.NS_bullet'"));
	if (NiagaraSystemAssetExp.Succeeded())
	{
		NiagaraComponentHit->SetAsset(NiagaraSystemAssetExp.Object);
		NiagaraComponentHit->bAutoActivate = true; // 컴포넌트가 자동 실행되도록 설정
	}
	Speed = 12000.f;
}

void AGCProjectileBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetActorLocation(GetActorLocation() + (GetActorForwardVector() * Speed * DeltaTime));

	if (Target.IsValid())
	{
		TargetDist = FVector::Dist(GetActorLocation(), TargetLocation);
		
		if (TargetDist < 150 && bLocked) 
		{
			bLocked = false;
			
			Target->SetDamage(Damage);
			
			Destroy();
			
		}
	}
	else
	{
		if (bLocked) 
		{
			bLocked = false;
			Destroy();
		}
		
	}
}

void AGCProjectileBullet::BeginPlay()
{
	Super::BeginPlay();
	bLocked = true;
	if (Target.IsValid())
	{
		TargetLocation = Target->GetActorLocation();
	}
}

void AGCProjectileBullet::SetTargetLocation(FVector NewTargetLocation)
{
	TargetLocation = NewTargetLocation;
}



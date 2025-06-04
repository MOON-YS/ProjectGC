// Fill out your copyright notice in the Description page of Project Settings.


#include "GCProjectileRAM.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GCEnemy.h"
#include "Kismet/KismetMathLibrary.h"
#include "GCGameActorManager.h"

AGCProjectileRAM::AGCProjectileRAM()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_AIM120(TEXT("/Script/Engine.SkeletalMesh'/Game/VigilanteContent/Projectiles/West_Missile_AIM120/SK_West_Missile_AIM120.SK_West_Missile_AIM120'"));
	if (SK_AIM120.Succeeded())
	{
		Mesh->SetSkeletalMesh(SK_AIM120.Object);
	}
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraSystemAsset(TEXT("/Script/Niagara.NiagaraSystem'/Game/VFX/NS_West_Missile_AIM121.NS_West_Missile_AIM121'"));
	if (NiagaraSystemAsset.Succeeded())
	{
		NiagaraComponentTrail->SetAsset(NiagaraSystemAsset.Object);
		NiagaraComponentTrail->bAutoActivate = true; // 컴포넌트가 자동 실행되도록 설정
	}
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraSystemAssetExp(TEXT("/Script/Niagara.NiagaraSystem'/Game/AllExplosions/Niagara/Medium/NS_Explosion_Medium_7.NS_Explosion_Medium_7'"));
	if (NiagaraSystemAssetExp.Succeeded())
	{
		NiagaraComponentHit->SetAsset(NiagaraSystemAssetExp.Object);
		NiagaraComponentHit->bAutoActivate = false; // 컴포넌트가 자동 실행되도록 설정
	}
	Capsule->SetCapsuleHalfHeight(290.0f);
	NiagaraComponentTrail->SetRelativeLocationAndRotation(FVector(-160.f, 0.f, 0.f), FRotator(180.f, 0.f, 0.f));
	NiagaraComponentHit->SetRelativeLocationAndRotation(FVector(250.f, 0.f, 0.f), FRotator(180.f, 0.f, 0.f));
	Mesh->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));
	Speed = 2000.0f;
	bLocked = false;
}

void AGCProjectileRAM::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(TimerHandleAfterLaunch, [this]() {
		Speed = 12000.0f;
		if (Target.IsValid())
		{
			LockedTarget = Target;
			LockedLocation = LockedTarget->GetActorLocation();

			TargetDist = FVector::Dist(GetActorLocation(), LockedLocation);
			EstimatedTime = TargetDist / Speed;
			TargetVector = LockedTarget->GetActorLocation() + (LockedTarget->GetDirection() * LockedTarget->GetSpeed() * EstimatedTime);
		}
		bLocked = true;
		}, 0.5f, false);
}

void AGCProjectileRAM::Explosion()
{
	Mesh->ToggleVisibility();
	NiagaraComponentHit->SetActive(true);
	GetWorld()->GetTimerManager().SetTimer(TimerHandleExplosion, FTimerDelegate::CreateLambda([this]() {
		Destroy();
		}), 10.0f, false);
	Speed = 0;
	NiagaraComponentTrail->SetActive(false);
	bLocked = false;
}

void AGCProjectileRAM::Tick(float DeltaTime)
{
	SetActorLocation(GetActorLocation() + (GetActorForwardVector() * Speed * DeltaTime));
	TargetDist = FVector::Dist(GetActorLocation(), TargetVector);
	if (bLocked)
	{
		LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetVector);
		NewRotation = FMath::RInterpTo(GetActorRotation(), LookAtRotation, DeltaTime, 10.f);
		//GCLOG(Warning, TEXT("%f %f %f : %f %f %f"), LookAtRotation.Yaw, LookAtRotation.Pitch, LookAtRotation.Roll, NewRotation.Yaw, NewRotation.Pitch, NewRotation.Roll);
		if (FMath::Abs((LookAtRotation.Yaw - NewRotation.Yaw)) > 90)
		{
			Explosion();

			if (LockedTarget.IsValid())
			{
				LockedTarget->SetDamage(Damage);
			}
		}
		SetActorRotation(NewRotation);
	}


	if (TargetDist < 125 && bLocked)
	{
		Explosion();

		if (LockedTarget.IsValid())
		{
			LockedTarget->SetDamage(Damage);
		}
	}

	if (bLocked && !(LockedTarget.IsValid()))
	{
		Mesh->ToggleVisibility();
		GetWorld()->GetTimerManager().SetTimer(TimerHandleExplosion, FTimerDelegate::CreateLambda([this]() {
			Destroy();
			}), 10.0f, false);
		Speed = 0;
		NiagaraComponentTrail->SetActive(false);
		bLocked = false;
	}
}

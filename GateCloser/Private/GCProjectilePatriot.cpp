// Fill out your copyright notice in the Description page of Project Settings.


#include "GCProjectilePatriot.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GCEnemy.h"
#include "Kismet/KismetMathLibrary.h"
#include "GCGameActorManager.h"

AGCProjectilePatriot::AGCProjectilePatriot()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MIM104(TEXT("/Script/Engine.SkeletalMesh'/Game/VigilanteContent/Projectiles/West_Missile_MIM104/SK_West_Missile_MIM104.SK_West_Missile_MIM104'"));
	if (SK_MIM104.Succeeded())
	{
		Mesh->SetSkeletalMesh(SK_MIM104.Object);
	}
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraSystemAsset(TEXT("/Script/Niagara.NiagaraSystem'/Game/VFX/NS_West_Missile_AIM120.NS_West_Missile_AIM120'"));
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
	NiagaraComponentTrail->SetRelativeLocationAndRotation(FVector(-250.f, 0.f, 0.f), FRotator(180.f, 0.f, 0.f));
	NiagaraComponentHit->SetRelativeLocationAndRotation(FVector(250.f, 0.f, 0.f), FRotator(180.f, 0.f, 0.f));
	Mesh->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));
	Speed = 2000.0f;
	bLocked = false;
}

void AGCProjectilePatriot::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(TimerHandleAfterLaunch, [this]() {
		Speed = 4000.0f;
		if (Target.IsValid())
		{
			LockedLocation = Target->GetActorLocation();

			TargetDist = FVector::Dist(GetActorLocation(), LockedLocation);
			EstimatedTime = TargetDist / Speed;
			TargetVector = Target->GetActorLocation() + (Target->GetDirection() * Target->GetSpeed() * EstimatedTime);
		}
		bLocked = true;
		}, 0.5f, false);
}

void AGCProjectilePatriot::Tick(float DeltaTime)
{
	SetActorLocation(GetActorLocation() + (GetActorForwardVector() * Speed * DeltaTime));
	TargetDist = FVector::Dist(GetActorLocation(), TargetVector);
	if (bLocked)
	{

		LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetVector);
		NewRotation = FMath::RInterpTo(GetActorRotation(), LookAtRotation, DeltaTime, 2.f);
		SetActorRotation(NewRotation);
	}


	if (TargetDist < 150 && bLocked)
	{
		Mesh->ToggleVisibility();
		NiagaraComponentHit->SetActive(true);
		GetWorld()->GetTimerManager().SetTimer(TimerHandleExplosion, FTimerDelegate::CreateLambda([this]() {
			Destroy();
			}), 10.0f, false);
		Speed = 0;
		NiagaraComponentTrail->SetActive(false);
		bLocked = false;

		if (Target.IsValid())
		{
			Target->SetDamage(Damage);
		}
	}

	if (bLocked && !(Target.IsValid()))
	{

		Target = Cast<AGCEnemy>(ActorManager->GetEnemyByMinDistance(GetActorLocation(), 4000.0f));
		LockedLocation = Target->GetActorLocation();

		TargetDist = FVector::Dist(GetActorLocation(), LockedLocation);
		EstimatedTime = TargetDist / Speed;
		TargetVector = Target->GetActorLocation() + (Target->GetDirection() * Target->GetSpeed() * EstimatedTime);
		
	}
}

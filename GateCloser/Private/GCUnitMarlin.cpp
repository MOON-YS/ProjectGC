// Fill out your copyright notice in the Description page of Project Settings.


#include "GCUnitMarlin.h"
#include "GCAllyAnimInstance.h"
#include "GCGameActorManager.h"
#include "GCEnemy.h"
#include "DrawDebugHelpers.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "GCProjectileBullet.h"
#include "GCUnitBuffComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

AGCUnitMarlin::AGCUnitMarlin()
{
	Type = EUnitType::MARLIN;
	BuffComponent->SetTypeOfUnit(Type);
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Mesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Unit/Ally/Marlin/30mmMarlin.30mmMarlin'"));
	if (SK_Mesh.Succeeded())
	{
		Mesh->SetSkeletalMesh(SK_Mesh.Object);
	}
	TSubclassOf<UAnimInstance> AnimClass;
	static ConstructorHelpers::FClassFinder<UAnimInstance> ABP_C(TEXT("/Script/Engine.AnimBlueprint'/Game/Unit/Ally/Marlin/30mmMarlin_Skeleton_AnimBlueprint.30mmMarlin_Skeleton_AnimBlueprint_C'"));
	if (ABP_C.Succeeded())
	{
		AnimClass = ABP_C.Class;
		Mesh->SetAnimClass(AnimClass);
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> SoundCueRef(TEXT("/Script/Engine.SoundCue'/Game/Sounds/Cues/Marlin_Shot_Cue.Marlin_Shot_Cue'"));
	if (SoundCueRef.Succeeded())
	{
		SoundCue = SoundCueRef.Object;
		AudioComponent->SetSound(SoundCue);
		AudioComponent->bAutoActivate = false;
	}
	Box->SetRelativeLocation(FVector(0.0f, 0.f, 100.0f));
	Box->SetBoxExtent(FVector(99.0f, 99.0f, 50.0f));
	SpawnMesh->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));
	Mesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));

	bPreviousRTFState = false;

	AttackCool = 1.0f;
	AttackPoint = 20.0f;
}

void AGCUnitMarlin::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	GCAnimInstance = Cast<UGCAllyAnimInstance>(Mesh->GetAnimInstance());
	GCAnimInstance->BindActor(Cast<AGCUnitBase>(this), Type);
}

void AGCUnitMarlin::SetTimer()
{
	SetAnimDeployed();
	PitchPos = Mesh->GetBoneLocation(TEXT("gun"), EBoneSpaces::WorldSpace);
	GetWorld()->GetTimerManager().SetTimer(RotatingHandler, this, &AGCUnitMarlin::Rotating, 0.1, true);
}

void AGCUnitMarlin::Attacking()
{
	PitchPos = Mesh->GetBoneLocation(TEXT("gun"), EBoneSpaces::WorldSpace);
	FRotator CurrentLoockDir = UKismetMathLibrary::FindLookAtRotation(PitchPos, LockedLocation);

	FString SocketName = TEXT("Fire");
	FVector StartLocation = Mesh->GetSocketLocation(*SocketName);
	FVector Scale(1.0f, 1.0f, 1.0f);
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(StartLocation);
	SpawnTransform.SetRotation(FQuat(CurrentLoockDir));
	SpawnTransform.SetScale3D(Scale);

	AActor* Bullet = GetWorld()->SpawnActor<AGCProjectileBullet>(AGCProjectileBullet::StaticClass(), SpawnTransform);
	AGCProjectileBullet* SpawnedBullet = Cast<AGCProjectileBullet>(Bullet);
	SpawnedBullet->SetTarget(CurrentTargetActor);
	SpawnedBullet->SetTargetLocation(LockedLocation);
	SpawnedBullet->SetDamage(AttackPoint * BuffComponent->GetAttackScaleBonus());
	AudioComponent->Play();
}

void AGCUnitMarlin::Rotating()
{
	TArray<AGCEnemy*> CloseEnemies = ActorManager->GetEnemyByClosestDistance(PitchPos, 10000);

	if (CloseEnemies.IsValidIndex(0))
	{
		CloseEnemies.Sort([&](const AActor& A, const AActor& B)
			{
				float DistanceA = FVector::Dist(A.GetActorLocation(), PitchPos);
				float DistanceB = FVector::Dist(B.GetActorLocation(), PitchPos);
				return DistanceA < DistanceB;
			});


		CurrentTargetActor = CloseEnemies[0];


		LockedLocation = CloseEnemies[0]->GetActorLocation();

		float TargetDist = FVector::Dist(PitchPos, LockedLocation);
		float EstimatedTime = TargetDist / 6000.0f;
		LockedLocation = CloseEnemies[0]->GetActorLocation() + (CloseEnemies[0]->GetDirection() * CloseEnemies[0]->GetSpeed() * EstimatedTime);

		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(PitchPos, LockedLocation);

		GCAnimInstance->SetTargetRotate(FRotator(TargetRotation.Yaw - (GetActorRotation().Yaw), 0.f, TargetRotation.Pitch));
	
		if (!bPreviousRTFState && GCAnimInstance->GetReadyToFire())
		{
			GetWorldTimerManager().SetTimer(AttackTimerHandler, this, &AGCUnitMarlin::Attacking, AttackCool*BuffComponent->GetAttackCoolScaleBonus(), true, .2f);
		}
		if (bPreviousRTFState && !GCAnimInstance->GetReadyToFire())
		{
			GetWorldTimerManager().ClearTimer(AttackTimerHandler);
		}

		bPreviousRTFState = GCAnimInstance->GetReadyToFire();
	}
	else
	{
		bPreviousRTFState = false;
	}

}
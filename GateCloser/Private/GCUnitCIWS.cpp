// Fill out your copyright notice in the Description page of Project Settings.


#include "GCUnitCIWS.h"
#include "GCAllyAnimInstance.h"
#include "GCGameActorManager.h"
#include "GCEnemy.h"
#include "DrawDebugHelpers.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "GCUnitBuffComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

AGCUnitCIWS::AGCUnitCIWS()
{
	Type = EUnitType::CIWS;
	BuffComponent->SetTypeOfUnit(Type);
	NiagaraComponentFire = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Fire"));
	NiagaraComponentFire->SetupAttachment(Mesh,FName("FirePos"));
	NiagaraComponentFire->SetRelativeScale3D(FVector(0.01f, 0.01f, 0.01f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Mesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Unit/Ally/CIWS/ciws.ciws'"));
	if (SK_Mesh.Succeeded())
	{
		Mesh->SetSkeletalMesh(SK_Mesh.Object);
	}
	TSubclassOf<UAnimInstance> AnimClass;
	static ConstructorHelpers::FClassFinder<UAnimInstance> ABP_C(TEXT("/Script/Engine.AnimBlueprint'/Game/Unit/Ally/CIWS/ciws_Skeleton_AnimBlueprint.ciws_Skeleton_AnimBlueprint_C'"));
	if (ABP_C.Succeeded())
	{
		AnimClass = ABP_C.Class;
		Mesh->SetAnimClass(AnimClass);
	}
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraSystemAssetExp(TEXT("/Script/Niagara.NiagaraSystem'/Game/VigilanteContent/Vehicles/East_Arty_ZU23/FX/NS_East_Arty_ZU23_Gunfire.NS_East_Arty_ZU23_Gunfire'"));
	if (NiagaraSystemAssetExp.Succeeded())
	{
		NiagaraComponentFire->SetAsset(NiagaraSystemAssetExp.Object);
		NiagaraComponentFire->bAutoActivate = false; 
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> SoundCueRef(TEXT("/Script/Engine.SoundCue'/Game/Sounds/Cues/CIWS_Shot_Cue.CIWS_Shot_Cue'"));
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

	AttackCool = 10.0f;
	AttackPoint = 1.5f;

	
}
void AGCUnitCIWS::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	GCAnimInstance = Cast<UGCAllyAnimInstance>(Mesh->GetAnimInstance());
	GCAnimInstance->BindActor(Cast<AGCUnitBase>(this), Type);
}

void AGCUnitCIWS::BeginPlay()
{
	Super::BeginPlay();
}

void AGCUnitCIWS::SetTimer()
{
	SetAnimDeployed();
	PitchPos = Mesh->GetBoneLocation(TEXT("main"), EBoneSpaces::WorldSpace);
	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandler, this, &AGCUnitCIWS::Attacking, 0.1, true);
}

void AGCUnitCIWS::DelayDamage()
{
	if (CurrentTargetActor.IsValid())
	{
		CurrentTargetActor->SetDamage(AttackPoint*BuffComponent->GetAttackScaleBonus());
	}
}


void AGCUnitCIWS::Attacking()
{
	TArray<AGCEnemy*> CloseEnemies = ActorManager->GetEnemyByClosestDistance(PitchPos, 10000*BuffComponent->RangeScaleBonus());

	if (CloseEnemies.IsValidIndex(0)) 
	{
		CloseEnemies.Sort([&](const AActor& A, const AActor& B)
			{
				float DistanceA = FVector::Dist(A.GetActorLocation(), PitchPos);
				float DistanceB = FVector::Dist(B.GetActorLocation(), PitchPos);
				return DistanceA < DistanceB;
			});

		
		CurrentTargetActor = CloseEnemies[0];
		
		
		FVector LockedLocation = CloseEnemies[0]->GetActorLocation();

		float TargetDist = FVector::Dist(PitchPos, LockedLocation);
		float EstimatedTime = TargetDist / 8000.0f;
		LockedLocation = CloseEnemies[0]->GetActorLocation() + (CloseEnemies[0]->GetDirection() * CloseEnemies[0]->GetSpeed() * EstimatedTime);
		
		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LockedLocation);

		GCAnimInstance->SetTargetRotate(FRotator(TargetRotation.Yaw - 90, 0.f, TargetRotation.Pitch));
		NiagaraComponentFire->SetVariableFloat(TEXT("User.LifeTime"), EstimatedTime);
		if (GCAnimInstance->GetReadyToFire()) 
		{
			if (!NiagaraComponentFire->IsActive()) 
			{
				NiagaraComponentFire->SetActive(true);
				AudioComponent->Play();
			}
			
			if (IsValid(CloseEnemies[0]))
			{
				FTimerHandle TmpHandler;
				GetWorld()->GetTimerManager().SetTimer(TmpHandler, this, &AGCUnitCIWS::DelayDamage, EstimatedTime, false);
			}
			else
			{
				if (NiagaraComponentFire->IsActive())
				{
					NiagaraComponentFire->SetActive(false);
					AudioComponent->Stop();

				}
			}


		}
		else 
		{
			if (NiagaraComponentFire->IsActive())
			{
				NiagaraComponentFire->SetActive(false);
				AudioComponent->Stop();

			}
		}
	}
	else
	{
		if (NiagaraComponentFire->IsActive())
		{
			NiagaraComponentFire->SetActive(false);
			AudioComponent->Stop();

		}
	}
}

void AGCUnitCIWS::ToggleSoundPlay()
{
	if (!AudioComponent->IsPlaying())
	{
		AudioComponent->Play();
	}
	else
	{
		AudioComponent->Stop();
	}
}

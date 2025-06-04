// Fill out your copyright notice in the Description page of Project Settings.


#include "GCUnitGateCloser.h"
#include "GCAllyAnimInstance.h"
#include "GCEnemy.h"
#include "GCGameActorManager.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "GCGameState.h"
#include "GCUnitBuffComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

AGCUnitGateCloser::AGCUnitGateCloser()
{
	bPreviousRTFState = false;
	PrimaryActorTick.bCanEverTick = false;
	Type = EUnitType::GATECLOSER;
	BuffComponent->SetTypeOfUnit(Type);
	MaxHP = 100;
	CurrentHP = MaxHP;

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_GC(TEXT("/Script/Engine.SkeletalMesh'/Game/MainBase/RailGun.RailGun'"));
	if (SK_GC.Succeeded())
	{
		Mesh->SetSkeletalMesh(SK_GC.Object);
	}
	TSubclassOf<UAnimInstance> AnimClass;
	static ConstructorHelpers::FClassFinder<UAnimInstance> ABP_C(TEXT("/Script/Engine.AnimBlueprint'/Game/MainBase/ABP_RailGun.ABP_RailGun_C'"));
	if (ABP_C.Succeeded())
	{
		AnimClass = ABP_C.Class;
		Mesh->SetAnimClass(AnimClass);
	}
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraSystemAsset(TEXT("/Script/Niagara.NiagaraSystem'/Game/Beam_and_laserFX01/Particles/Niagara/NS_ky_laser03.NS_ky_laser03'"));
	if (NiagaraSystemAsset.Succeeded())
	{
		NiagaraComponent->SetAsset(NiagaraSystemAsset.Object);
		NiagaraComponent->bAutoActivate = false; // 컴포넌트가 자동 실행되도록 설정
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> SoundCueRef(TEXT("/Script/Engine.SoundCue'/Game/Sounds/Cues/GateCloser_Shot_Cue.GateCloser_Shot_Cue'"));
	if (SoundCueRef.Succeeded())
	{
		SoundCue = SoundCueRef.Object;
		AudioComponent->SetSound(SoundCue);
		AudioComponent->bAutoActivate = false;
	}
	//NiagaraComponent->AttachToComponent(Mesh, FAttachmentTransformRules::KeepWorldTransform, TEXT("Socket_Front"));

	Box->SetBoxExtent(FVector(399.0f, 399.0f, 200.0f));
	Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, -200.0f));
	Mesh->SetRelativeScale3D(FVector(3.f, 3.f, 3.f));

	SpawnMesh->DestroyComponent();

	AttackCool = 3.0f;
	AttackPoint = 50.0f;
}
void AGCUnitGateCloser::Fire()
{

	if (CurrentTargetActor.IsValid()) 
	{
		FVector TargetNormalVector = (CurrentTargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		float DotProduct = FVector::DotProduct(TargetNormalVector, GetActorForwardVector().GetSafeNormal());
		float AngleRadians = FMath::Acos(DotProduct);
		float AngleDegrees = FMath::RadiansToDegrees(AngleRadians);
		if (TargetNormalVector.Y < 0 && GetActorRotation().Yaw == 0) {
			AngleDegrees = 360 - AngleDegrees;
		}
		else AngleDegrees = 360 + AngleDegrees;

		FVector BeamStartLocation = Mesh->GetSocketLocation(TEXT("Socket_Front"));
		BeamStartLocation.Z = 25.0f;
		NiagaraComponent->SetRelativeLocation(BeamStartLocation);
		NiagaraComponent->SetRelativeRotation(FRotator(0.0f, AngleDegrees, 0.0f));
		FVector NewBeamEndValue(FVector::Dist(BeamStartLocation, CurrentTargetActor->GetActorLocation()) - 10, 0.0f, CurrentTargetActor->GetActorLocation().Z - 200.f);
		NiagaraComponent->SetVariableVec3(TEXT("User.beamEnd"), NewBeamEndValue);
		NiagaraComponent->Activate();

		GetWorld()->GetTimerManager().SetTimer(DelayDamageHandle, this, &AGCUnitGateCloser::DelayedDamage, 0.1f, false);
		AudioComponent->Play();
	}
	else
	{
		GetWorldTimerManager().ClearTimer(AttackTimerHandler);
	}

}
void AGCUnitGateCloser::SetTarget()
{
	TArray<AGCEnemy*> CloseEnemies = ActorManager->GetEnemyByClosestDistance(FVector::ZeroVector, 100000);

	if (CloseEnemies.IsValidIndex(0))
	{
		CloseEnemies.Sort([&](const AActor& A, const AActor& B)
			{
				float DistanceA = FVector::Dist(A.GetActorLocation(), FVector::ZeroVector);
				float DistanceB = FVector::Dist(B.GetActorLocation(), FVector::ZeroVector);
				return DistanceA < DistanceB;
			});


		CurrentTargetActor = CloseEnemies[0];


		LockedLocation = CloseEnemies[0]->GetActorLocation();

		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(FVector::ZeroVector, LockedLocation);
		float TargetYaw;
		if (TargetRotation.Yaw - (GetActorRotation().Yaw + 90) > 360)
		{
			TargetYaw = TargetRotation.Yaw - (GetActorRotation().Yaw + 90) - 360;
		}
		else
		{
			TargetYaw = TargetRotation.Yaw - (GetActorRotation().Yaw + 90);
		}
		GCAnimInstance->SetTargetRotate(FRotator(0.f, TargetYaw, 0.f));


		if (!bPreviousRTFState && GCAnimInstance->GetReadyToFire())
		{
			GetWorldTimerManager().SetTimer(AttackTimerHandler, this, &AGCUnitGateCloser::Fire, AttackCool*BuffComponent->GetAttackCoolScaleBonus(), true, 3.0f);
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

void AGCUnitGateCloser::SetDamage(float Damage)
{
	CurrentHP -= Damage;
	GameState->SetCurrentHp(CurrentHP);
	
	//TODO: 사망 처리
}

void AGCUnitGateCloser::DelayedDamage()
{
	if (CurrentTargetActor.IsValid())
	{
		CurrentTargetActor->SetDamage(AttackPoint * BuffComponent->GetAttackScaleBonus());
		GCLOG(Warning, TEXT("%f"), AttackPoint * BuffComponent->GetAttackScaleBonus());
	}
}

void AGCUnitGateCloser::BeginPlay()
{
	Super::BeginPlay();
	GCAnimInstance->SetDeployed();
	GetWorld()->GetTimerManager().SetTimer(RotatingHandler, this, &AGCUnitGateCloser::SetTarget, 0.1f, true, 2.0f);
	GameState = Cast<AGCGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (IsValid(GameState))
	{
		GameState->SetMaxHp(MaxHP);
		GameState->SetCurrentHp(MaxHP);
	}

}

void AGCUnitGateCloser::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	GCAnimInstance = Cast<UGCAllyAnimInstance>(Mesh->GetAnimInstance());
	GCAnimInstance->BindActor(Cast<AGCUnitBase>(this), Type);


}

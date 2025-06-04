// Fill out your copyright notice in the Description page of Project Settings.


#include "GCUnitM1A1.h"
#include "GCAllyAnimInstance.h"
#include "GCGameActorManager.h"
#include "GCEnemy.h"
#include "Kismet/KismetMathLibrary.h"
#include "GCProjectileBullet.h"
#include "DrawDebugHelpers.h"
#include "GCUnitBuffComponent.h"
#include "GCGameInstance.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

AGCUnitM1A1::AGCUnitM1A1()
{
	Type = EUnitType::M1A1;
	BuffComponent->SetTypeOfUnit(Type);
	CascadeParticleComponentFire = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("CascadeParticleComponent"));
	CascadeParticleComponentFire->SetupAttachment(Mesh, FName("Fire"));

	CascadeParticleComponentSmoke = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("CascadeParticleComponentSmoke"));
	CascadeParticleComponentSmoke->SetupAttachment(RootComponent);
	CascadeParticleComponentSmoke->SetRelativeLocation(FVector(0.0f, 0.f, -50.0f));
	CascadeParticleComponentSmoke->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Mesh(TEXT("/Script/Engine.SkeletalMesh'/Game/VigilanteContent/Vehicles/West_Tank_M1A1Abrams/SK_West_Tank_M1A1Abrams.SK_West_Tank_M1A1Abrams'"));
	if (SK_Mesh.Succeeded())
	{
		Mesh->SetSkeletalMesh(SK_Mesh.Object);
	}
	Box->SetRelativeLocation(FVector(0.0f, 0.f, 100.0f));
	Box->SetBoxExtent(FVector(149.0f, 99.0f, 50.0f));
	SpawnMesh->SetRelativeScale3D(FVector(3.f, 2.f, 2.f));
	Mesh->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));
	ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemFinder(TEXT("/Script/Engine.ParticleSystem'/Game/VigilanteContent/Vehicles/West_Tank_M1A1Abrams/FX/PS_MuzzleFire_01_M1A1Abrams.PS_MuzzleFire_01_M1A1Abrams'"));
	if (ParticleSystemFinder.Succeeded())
	{
		CascadeParticleComponentFire->SetTemplate(ParticleSystemFinder.Object);
		CascadeParticleComponentFire->bAutoActivate = false;
	}
	ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemFinderSW(TEXT("/Script/Engine.ParticleSystem'/Game/VigilanteContent/Vehicles/West_Tank_M1A1Abrams/FX/PS_ShockWave_M1A1Abrams.PS_ShockWave_M1A1Abrams'"));
	if (ParticleSystemFinderSW.Succeeded())
	{
		CascadeParticleComponentSmoke->SetTemplate(ParticleSystemFinderSW.Object);
		CascadeParticleComponentSmoke->bAutoActivate = false;
	}

	TSubclassOf<UAnimInstance> AnimClass;
	static ConstructorHelpers::FClassFinder<UAnimInstance> ABP_C(TEXT("/Script/Engine.AnimBlueprint'/Game/VigilanteContent/Vehicles/West_Tank_M1A1Abrams/SKEL_West_Tank_M1A1Abrams_AnimBlueprint.SKEL_West_Tank_M1A1Abrams_AnimBlueprint_C'"));
	if (ABP_C.Succeeded())
	{
		AnimClass = ABP_C.Class;
		Mesh->SetAnimClass(AnimClass);
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> SoundCueRef(TEXT("/Script/Engine.SoundCue'/Game/Sounds/Cues/M1A1_Shot_Cue.M1A1_Shot_Cue'"));
	if (SoundCueRef.Succeeded())
	{
		SoundCue = SoundCueRef.Object;
		AudioComponent->SetSound(SoundCue);
		AudioComponent->bAutoActivate = false;
	}
	AttackCool = 5.0f;
	AttackCoolScale = 1.0f;
	AttackPoint = 70.0f;
	AttackPointScale = 1.0f;
}

void AGCUnitM1A1::SetTarget()
{
	PitchPos = Mesh->GetBoneLocation(TEXT("gun_jnt"), EBoneSpaces::WorldSpace);
	
	TArray<AGCEnemy*> CloseEnemies = ActorManager->GetEnemyByClosestDistance(PitchPos, 20000* BuffComponent->RangeScaleBonus());
	CloseEnemies.Sort([&](const AActor& A, const AActor& B)
		{
			float DistanceA = FVector::Dist(A.GetActorLocation(), PitchPos);
			float DistanceB = FVector::Dist(B.GetActorLocation(), PitchPos);
			return DistanceA < DistanceB;
		});
	for (AGCEnemy* Enemy : CloseEnemies)
	{
		if (IsValid(Enemy)) 
		{
			CurrentTargetActor = Enemy;
			LockedLocation = Enemy->GetActorLocation();

			float TargetDist = FVector::Dist(PitchPos, LockedLocation);
			float EstimatedTime = TargetDist / 6000.0f;
			FRotator InitTargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LockedLocation);

			float RotDist = FMath::Abs(GCAnimInstance->GetCurrentRotation().Pitch - InitTargetRotation.Yaw);
			float EstimatedRotationTime = RotDist / 150.f;
			
			LockedLocation = Enemy->GetActorLocation() + (Enemy->GetDirection() * Enemy->GetSpeed() * (EstimatedTime+0.2f+ EstimatedRotationTime));

			CurrentLoockDir = UKismetMathLibrary::FindLookAtRotation(PitchPos, LockedLocation);
			
			GCAnimInstance->SetTargetRotate(FRotator(CurrentLoockDir.Yaw-GetActorRotation().Yaw, 0.f, CurrentLoockDir.Pitch));
			GCAnimInstance->SetReadyToFire();

			GCLOG(Warning, TEXT("Targeting"));
			break;
		}
	}
}

void AGCUnitM1A1::Fire()
{
	GetWorld()->GetTimerManager().SetTimer(AttackDelayHandle, this, &AGCUnitM1A1::DelayedFire, 0.2f, false);
	
}

void AGCUnitM1A1::DelayedFire()
{

	CascadeParticleComponentFire->Deactivate();
	CascadeParticleComponentFire->Activate();
	PitchPos = Mesh->GetBoneLocation(TEXT("gun_jnt"), EBoneSpaces::WorldSpace);

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
	SpawnedBullet->SetDamage(AttackPoint*BuffComponent->GetAttackCoolScaleBonus());
	AudioComponent->Play();
}

void AGCUnitM1A1::SetTimer()
{

	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandler, this, &AGCUnitM1A1::SetTarget, AttackCool*AttackCoolScale, true,0.2);
}

void AGCUnitM1A1::ResetTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandler);
	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandler, this, &AGCUnitM1A1::SetTarget, AttackCool * BuffComponent->GetAttackCoolScaleBonus(), true, 0.2);
}

void AGCUnitM1A1::BeginPlay()
{
	Super::BeginPlay();
	auto GameInstance = Cast<UGCGameInstance>(GetGameInstance());
	GameInstance->OnCoolChangedRAM.AddUObject(this, &AGCUnitM1A1::ResetTimer);
}

void AGCUnitM1A1::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	GCAnimInstance = Cast<UGCAllyAnimInstance>(Mesh->GetAnimInstance());
	GCAnimInstance->BindActor(Cast<AGCUnitBase>(this), Type);
}
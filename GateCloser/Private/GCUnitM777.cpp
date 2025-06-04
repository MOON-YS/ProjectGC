// Fill out your copyright notice in the Description page of Project Settings.


#include "GCUnitM777.h"
#include "GCAllyAnimInstance.h"
#include "GCGameActorManager.h"
#include "Particles/ParticleSystemComponent.h"
#include "GCExplosion.h"
#include "GCEnemy.h"
#include "GCUnitBuffComponent.h"
#include "GCGameInstance.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

AGCUnitM777::AGCUnitM777()
{
	PrimaryActorTick.bCanEverTick = false;

	Type = EUnitType::M777;
	BuffComponent->SetTypeOfUnit(Type);
	AttackCool = 10.0f;
	AttackCoolScale = 1.0f;
	AttackPoint = 40.0f;
	AttackPointScale = 1.0f;

	CascadeParticleComponentFire = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("CascadeParticleComponent"));
	CascadeParticleComponentFire->SetupAttachment(Mesh, FName("Socket_Fire"));
	CascadeParticleComponentFire->SetRelativeRotation(FRotator(-180.f, 0.f, 0.f));
	
	CascadeParticleComponentSmoke = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("CascadeParticleComponentSmoke"));
	CascadeParticleComponentSmoke->SetupAttachment(RootComponent); 
	CascadeParticleComponentSmoke->SetRelativeLocation(FVector(0.0f, 0.f, -50.0f));
	CascadeParticleComponentSmoke->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Mesh(TEXT("/Script/Engine.SkeletalMesh'/Game/VigilanteContent/Vehicles/West_Arty_M777/SK_West_Arty_M777.SK_West_Arty_M777'"));
	if (SK_Mesh.Succeeded())
	{
		Mesh->SetSkeletalMesh(SK_Mesh.Object);
	}
	TSubclassOf<UAnimInstance> AnimClass;
	static ConstructorHelpers::FClassFinder<UAnimInstance> ABP_C(TEXT("/Script/Engine.AnimBlueprint'/Game/Animations/M777/ABP_M777.ABP_M777_C'"));
	if (ABP_C.Succeeded())
	{
		AnimClass = ABP_C.Class;
		Mesh->SetAnimClass(AnimClass);
	}
	ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemFinder(TEXT("/Script/Engine.ParticleSystem'/Game/VigilanteContent/Vehicles/West_Arty_M777/FX/PS_CannonExplosion_01_West_Arty_M777.PS_CannonExplosion_01_West_Arty_M777'"));
	if (ParticleSystemFinder.Succeeded())
	{
		CascadeParticleComponentFire->SetTemplate(ParticleSystemFinder.Object);
		CascadeParticleComponentFire->bAutoActivate = false;
	}
	ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemFinderSW(TEXT("/Script/Engine.ParticleSystem'/Game/VigilanteContent/Vehicles/West_Arty_M777/FX/PS_CannonShockwave_01_West_Arty_M777.PS_CannonShockwave_01_West_Arty_M777'"));
	if (ParticleSystemFinderSW.Succeeded())
	{
		CascadeParticleComponentSmoke->SetTemplate(ParticleSystemFinderSW.Object);
		CascadeParticleComponentSmoke->bAutoActivate = false;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> SoundCueRef(TEXT("/Script/Engine.SoundCue'/Game/Sounds/Cues/M777_Shot_Cue.M777_Shot_Cue'"));
	if (SoundCueRef.Succeeded())
	{
		SoundCue = SoundCueRef.Object;
		AudioComponent->SetSound(SoundCue);
		AudioComponent->bAutoActivate = false;
	}
	Box->SetRelativeLocation(FVector(0.0f, 0.f, 100.0f));
	Box->SetBoxExtent(FVector(99.0f, 99.0f, 50.0f));
	SpawnMesh->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));

}



void AGCUnitM777::Fire()
{
	CascadeParticleComponentSmoke->Deactivate();
	CascadeParticleComponentSmoke->Activate();
	CascadeParticleComponentFire->Deactivate();
	CascadeParticleComponentFire->Activate();
	TargetLocation.Z = 30.f;
	Explosion->SetActorLocation(TargetLocation);
	Explosion->PlayExplosionStart();
	AudioComponent->Play();
}

void AGCUnitM777::SetRotate()
{
	CurrentTargetActor = Cast<AGCEnemy>(ActorManager->GetEnemyByMinDistance(GetActorLocation(), 20000.f));
	if (CurrentTargetActor.IsValid())
	{
		GCAnimInstance->SetTargetRotate(GetTargetRotate(CurrentTargetActor));
		TargetLocation = CurrentTargetActor->GetActorLocation();
		GCAnimInstance->SetReadyToFire();
	}
}

void AGCUnitM777::SetTimer()
{
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(FVector::ZeroVector);
	SpawnTransform.SetRotation(FQuat(FRotator::ZeroRotator));
	SpawnTransform.SetScale3D(FVector(1.0f,1.0f,1.0f));
	AActor* SpawnActor = GetWorld()->SpawnActor<AGCExplosion>(AGCExplosion::StaticClass(), SpawnTransform);
	Explosion = Cast<AGCExplosion>(SpawnActor);
	Explosion->BindActorManager(ActorManager);
	Explosion->SetDamage(AttackPoint * AttackPointScale);
	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandler, this, &AGCUnitM777::SetRotate, AttackCool*BuffComponent->GetAttackCoolScaleBonus(), true,0.2);
}

void AGCUnitM777::ResetTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandler);
	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandler, this, &AGCUnitM777::SetRotate, AttackCool * BuffComponent->GetAttackCoolScaleBonus(), true, 0.2);

}

void AGCUnitM777::ResetExplosion()
{
	Explosion->SetDamage(AttackPoint * BuffComponent->GetAttackScaleBonus());
	Explosion->SetExplosionRange(Explosion->GetExplosionRange() * BuffComponent->RangeScaleBonus());

}

void AGCUnitM777::BeginPlay()
{
	Super::BeginPlay();
	auto GameInstance = Cast<UGCGameInstance>(GetGameInstance());
	GameInstance->OnCoolChangedRAM.AddUObject(this, &AGCUnitM777::ResetTimer);
}

void AGCUnitM777::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	GCAnimInstance = Cast<UGCAllyAnimInstance>(Mesh->GetAnimInstance());
	GCAnimInstance->BindActor(Cast<AGCUnitBase>(this), Type);
}

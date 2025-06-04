// Fill out your copyright notice in the Description page of Project Settings.


#include "GCUnitPatriot.h"
#include "GCAllyAnimInstance.h"
#include "GCProjectilePatriot.h"
#include "GCGameActorManager.h"
#include "GCEnemy.h"
#include "GCUnitBuffComponent.h"
#include "GCGameInstance.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

AGCUnitPatriot::AGCUnitPatriot()
{
	PrimaryActorTick.bCanEverTick = false;

	Type = EUnitType::PATRIOT;
	BuffComponent->SetTypeOfUnit(Type);
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_PATRIOT(TEXT("/Script/Engine.SkeletalMesh'/Game/VigilanteContent/Vehicles/West_AA_Patriot/SK_West_AA_Patriot.SK_West_AA_Patriot'"));
	if (SK_PATRIOT.Succeeded())
	{
		Mesh->SetSkeletalMesh(SK_PATRIOT.Object);
	}
	TSubclassOf<UAnimInstance> AnimClass;
	static ConstructorHelpers::FClassFinder<UAnimInstance> ABP_C(TEXT("/Script/Engine.AnimBlueprint'/Game/Animations/Patriot/ABP_Patriot.ABP_Patriot_C'"));
	if (ABP_C.Succeeded())
	{
		AnimClass = ABP_C.Class;
		Mesh->SetAnimClass(AnimClass);
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> SoundCueRef(TEXT("/Script/Engine.SoundCue'/Game/Sounds/Cues/Patriot_Shot_Cue.Patriot_Shot_Cue'"));
	if (SoundCueRef.Succeeded())
	{
		SoundCue = SoundCueRef.Object;
		AudioComponent->SetSound(SoundCue);
		AudioComponent->bAutoActivate = false;
	}
	Box->SetRelativeLocation(FVector(0.0f, 0.f, 100.0f));
	Box->SetBoxExtent(FVector(199.0f, 99.0f, 50.0f));

	AttackCool = 10.0f;
	AttackCoolScale = 1.0f;
	AttackPoint = 50.0f;
	AttackPointScale = 1.0f;

}

void AGCUnitPatriot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGCUnitPatriot::Fire()
{
	GEngine->ForceGarbageCollection();
	if (CurrentTargetActor.IsValid())
	{
		FVector StartLocation = Mesh->GetSocketLocation(TEXT("Socket_Launch1"));
		FRotator Rotation = FRotator(40.f, GetTargetRotate(CurrentTargetActor).Yaw + GetActorRotation().Yaw, 0.0f);
		FVector Scale(1.0f, 1.0f, 1.0f);
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(StartLocation);
		SpawnTransform.SetRotation(FQuat(Rotation));
		SpawnTransform.SetScale3D(Scale);
		AActor* Missile = GetWorld()->SpawnActor<AGCProjectilePatriot>(AGCProjectilePatriot::StaticClass(), SpawnTransform);
		AGCProjectilePatriot* SpawnedMissile = Cast<AGCProjectilePatriot>(Missile);
		SpawnedMissile->SetTarget(CurrentTargetActor);
		SpawnedMissile->BindActorMananger(ActorManager);
		SpawnedMissile->SetDamage(AttackPoint * BuffComponent->GetAttackScaleBonus());
		AudioComponent->Play();
	}
}

void AGCUnitPatriot::SetRotate()
{
	CurrentTargetActor = Cast<AGCEnemy>(ActorManager->GetEnemyByMinDistance(GetActorLocation(), 20000.f));
	if (CurrentTargetActor.IsValid())
	{
		GCAnimInstance->SetTargetRotate(GetTargetRotate(CurrentTargetActor));
		GCAnimInstance->SetReadyToFire();
	}
}

void AGCUnitPatriot::SetTimer()
{
	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandler, this, &AGCUnitPatriot::SetRotate, AttackCool, true,0.2);
}

void AGCUnitPatriot::ResetTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandler);
	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandler, this, &AGCUnitPatriot::SetRotate, AttackCool * BuffComponent->GetAttackCoolScaleBonus(), true, 0.2);
}

void AGCUnitPatriot::BeginPlay()
{
	Super::BeginPlay();
	auto GameInstance = Cast<UGCGameInstance>(GetGameInstance());
	GameInstance->OnCoolChangedRAM.AddUObject(this, &AGCUnitPatriot::ResetTimer);
}

void AGCUnitPatriot::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	GCAnimInstance = Cast<UGCAllyAnimInstance>(Mesh->GetAnimInstance());
	GCAnimInstance->BindActor(Cast<AGCUnitBase>(this),Type);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "GCUnitRAM.h"
#include "GCAllyAnimInstance.h"
#include "GCGameActorManager.h"
#include "GCProjectilePatriot.h"
#include "GCEnemy.h"
#include "Kismet/KismetMathLibrary.h"
#include "Containers/Map.h"
#include "DrawDebugHelpers.h"
#include "GCProjectileRAM.h"
#include "GCUnitBuffComponent.h"
#include "GCGameInstance.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

AGCUnitRAM::AGCUnitRAM()
{
	Type = EUnitType::SEARAM;
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Mesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Unit/Ally/SeaRAM/searam.searam'"));
	if (SK_Mesh.Succeeded())
	{
		Mesh->SetSkeletalMesh(SK_Mesh.Object);
	}
	Box->SetRelativeLocation(FVector(0.0f, 0.f, 100.0f));
	Box->SetBoxExtent(FVector(99.0f, 99.0f, 50.0f));
	SpawnMesh->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));
	Mesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	TSubclassOf<UAnimInstance> AnimClass;
	static ConstructorHelpers::FClassFinder<UAnimInstance> ABP_C(TEXT("/Script/Engine.AnimBlueprint'/Game/Unit/Ally/SeaRAM/searam_Skeleton_AnimBlueprint.searam_Skeleton_AnimBlueprint_C'"));
	if (ABP_C.Succeeded())
	{
		AnimClass = ABP_C.Class;
		Mesh->SetAnimClass(AnimClass);
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> SoundCueRef(TEXT("/Script/Engine.SoundCue'/Game/Sounds/Cues/RAM_Shot_Cue.RAM_Shot_Cue'"));
	if (SoundCueRef.Succeeded())
	{
		SoundCue = SoundCueRef.Object;
		AudioComponent->SetSound(SoundCue);
		AudioComponent->bAutoActivate = false;
	}
	

	BuffComponent->SetTypeOfUnit(Type);
	Box->SetRelativeLocation(FVector(0.0f, 0.f, 100.0f));
	Box->SetBoxExtent(FVector(99.0f, 99.0f, 50.0f));
	SpawnMesh->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));
	Mesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));

	EnabledMissileCount = 3;
	MissileFireRepeat = 0;

	AttackCool = 10.0f;
	AttackPoint = 20.0f;

}

void AGCUnitRAM::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	GCAnimInstance = Cast<UGCAllyAnimInstance>(Mesh->GetAnimInstance());
	GCAnimInstance->BindActor(Cast<AGCUnitBase>(this), Type);
}

void AGCUnitRAM::BeginPlay()
{
	Super::BeginPlay();
	auto GameInstance = Cast<UGCGameInstance>(GetGameInstance());
	GameInstance->OnCoolChangedRAM.AddUObject(this, &AGCUnitRAM::ResetTimer);
}

void AGCUnitRAM::SetTimer()
{
	SetAnimDeployed();
	//타깃설정 타이머
	PitchPos = Mesh->GetBoneLocation(TEXT("main"), EBoneSpaces::WorldSpace);
	SetTarget();
	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandler, this, &AGCUnitRAM::SetTarget, AttackCool, true);
}

void AGCUnitRAM::Fire()
{
	//GCLOG(Warning, TEXT("FireStart"));
	MissileFireRepeat = 0;
	GetWorld()->GetTimerManager().SetTimer(FireMissileTimerHandle, this, &AGCUnitRAM::FireMissiles, 0.1f, true);
	
}

void AGCUnitRAM::FireMissiles()
{
	//사격 매커니즘 작성
	//GCLOG(Warning, TEXT("Fire %d Missle"),MissileFireRepeat+1);
	PitchPos = Mesh->GetBoneLocation(TEXT("main"), EBoneSpaces::WorldSpace);
	TArray<AGCEnemy*> CloseEnemies = ActorManager->GetEnemyByClosestDistance(PitchPos, 100000);
	CloseEnemies.Sort([&](const AActor& A, const AActor& B)
		{
			float DistanceA = FVector::Dist(A.GetActorLocation(), PitchPos);
			float DistanceB = FVector::Dist(B.GetActorLocation(), PitchPos);
			return DistanceA < DistanceB;
		});

	CloseEnemies = CloseEnemies.FilterByPredicate([this](AGCEnemy* Enemy) {
		FVector EnemyDir = Enemy->GetActorLocation() - PitchPos;
		FVector NomalEnemyDir = EnemyDir.GetSafeNormal();
		FVector NomalCurrentLoockDir = CurrentLoockDir.GetSafeNormal();

		float DotProduct = FVector::DotProduct(NomalEnemyDir, NomalCurrentLoockDir);
		float AngleInRadians = FMath::Acos(DotProduct);

		return (FMath::RadiansToDegrees(AngleInRadians) < 15.0f);
	
		});

	if (CloseEnemies.IsValidIndex(MissileFireRepeat))
	{
		FString SocketName = TEXT("Fire_") + FString::FromInt(MissileFireRepeat+1);
		FVector StartLocation = Mesh->GetSocketLocation(*SocketName);
		FRotator Rotation = FRotator(CurrentLoockDir.Rotation());
		FVector Scale(1.0f, 1.0f, 1.0f);
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(StartLocation);
		SpawnTransform.SetRotation(FQuat(Rotation));
		SpawnTransform.SetScale3D(Scale);
		AActor* Missile = GetWorld()->SpawnActor<AGCProjectileRAM>(AGCProjectileRAM::StaticClass(), SpawnTransform);
		AGCProjectileRAM* SpawnedMissile = Cast<AGCProjectileRAM>(Missile);
		SpawnedMissile->SetTarget(CloseEnemies[MissileFireRepeat]);
		SpawnedMissile->BindActorMananger(ActorManager);
		SpawnedMissile->SetDamage(AttackPoint*BuffComponent->GetAttackScaleBonus());
		AudioComponent->Play();
	}
	else
	{
		if (CloseEnemies.Num() == 0) { 
			MissileFireRepeat++;
			if (MissileFireRepeat == EnabledMissileCount+BuffComponent->GetAdditionalProjectile())
			{
				GetWorld()->GetTimerManager().ClearTimer(FireMissileTimerHandle);
				//GCLOG(Warning, TEXT("FireEnd"));
			}
			return;
		}
		int32 Num = MissileFireRepeat % CloseEnemies.Num();
		if (CloseEnemies.IsValidIndex(Num))
		{
			FString SocketName = TEXT("Fire_") + FString::FromInt(MissileFireRepeat + 1);
			FVector StartLocation = Mesh->GetSocketLocation(*SocketName);
			FRotator Rotation = FRotator(CurrentLoockDir.Rotation());
			FVector Scale(1.0f, 1.0f, 1.0f);
			FTransform SpawnTransform;
			SpawnTransform.SetLocation(StartLocation);
			SpawnTransform.SetRotation(FQuat(Rotation));
			SpawnTransform.SetScale3D(Scale);
			AActor* Missile = GetWorld()->SpawnActor<AGCProjectileRAM>(AGCProjectileRAM::StaticClass(), SpawnTransform);
			AGCProjectileRAM* SpawnedMissile = Cast<AGCProjectileRAM>(Missile);
			SpawnedMissile->SetTarget(CloseEnemies[Num]);
			SpawnedMissile->BindActorMananger(ActorManager);
			AudioComponent->Play();
			
		}
		else
		{
			GetWorld()->GetTimerManager().ClearTimer(FireMissileTimerHandle);
			//GCLOG(Warning, TEXT("FireEnd"));
		}
	}

	if (MissileFireRepeat + 1 == EnabledMissileCount+BuffComponent->GetAdditionalProjectile())
	{
		GetWorld()->GetTimerManager().ClearTimer(FireMissileTimerHandle);
		//GCLOG(Warning, TEXT("FireEnd"));
	}

	MissileFireRepeat++;
}
//첫 타깃 설정
void AGCUnitRAM::SetTarget()
{
	PitchPos = Mesh->GetBoneLocation(TEXT("main"), EBoneSpaces::WorldSpace);
	TArray<AGCEnemy*> CloseEnemies = ActorManager->GetEnemyByClosestDistance(PitchPos, 100000);
	
	if (CloseEnemies.IsValidIndex(0))
	{
		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(PitchPos, CloseEnemies[0]->GetActorLocation());
		CurrentLoockDir =  CloseEnemies[0]->GetActorLocation() - PitchPos;
		GCAnimInstance->SetTargetRotate(FRotator(TargetRotation.Yaw - (GetActorRotation().Yaw+90), 0.f, TargetRotation.Pitch));
		GCAnimInstance->SetReadyToFire();
	}
}

void AGCUnitRAM::ResetTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandler);
	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandler, this, &AGCUnitRAM::SetTarget, AttackCool * BuffComponent->GetAttackCoolScaleBonus(), true);
}

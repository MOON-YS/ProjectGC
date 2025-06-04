// Fill out your copyright notice in the Description page of Project Settings.


#include "GCGameActorManager.h"
#include "GCGameMode.h"
#include "GCUnitBase.h"
#include "GCUnitPatriot.h"
#include "GCUnitGateCloser.h"
#include "GCUnitM777.h"
#include "GCUnitCIWS.h"
#include "GCUnitRAM.h"
#include "GCUnitM1A1.h"
#include "GCUnitMarlin.h"
#include "GCEnemy.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "GCGameState.h"

// Sets default values
AGCGameActorManager::AGCGameActorManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

AActor* AGCGameActorManager::SpawnAI(FVector SpawnPos, FRotator SpawnRotation, EUnitType AIType)
{

	FVector Scale(1.0f, 1.0f, 1.0f);
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnPos);
	SpawnTransform.SetRotation(FQuat(SpawnRotation));
	SpawnTransform.SetScale3D(Scale);
	SpawnedActor = nullptr;
	switch (AIType) 
	{
	case EUnitType::PATRIOT:
	{
		SpawnedActor = World->SpawnActor<AGCUnitPatriot>(AGCUnitPatriot::StaticClass(), SpawnTransform);
		if (SpawnedActor != nullptr)
		{
			AGCUnitPatriot* Patriot = Cast<AGCUnitPatriot>(SpawnedActor);
			if (Patriot != nullptr) Patriot->BindActorManager(this);
			else GCLOG(Error, TEXT("Can't Cast Patriot"));
		}
		else GCLOG(Error, TEXT("Can't spawn Patriot"));
		
		break;
	}
	case EUnitType::M777:
	{
		SpawnedActor = World->SpawnActor<AGCUnitM777>(AGCUnitM777::StaticClass(), SpawnTransform);
		if (SpawnedActor != nullptr)
		{
			AGCUnitM777* M777 = Cast<AGCUnitM777>(SpawnedActor);
			if (M777 != nullptr) M777->BindActorManager(this);
			else GCLOG(Error, TEXT("Can't Cast M777"));
		}
		else GCLOG(Error, TEXT("Can't spawn M777"));
		break;
	}
	case EUnitType::M1A1:
	{
		SpawnedActor = World->SpawnActor<AGCUnitM1A1>(AGCUnitM1A1::StaticClass(), SpawnTransform);
		if (SpawnedActor != nullptr)
		{
			AGCUnitM1A1* M1A1 = Cast<AGCUnitM1A1>(SpawnedActor);
			if (M1A1 != nullptr) M1A1->BindActorManager(this);
			else GCLOG(Error, TEXT("Can't Cast M1A1"));
		}
		break;
	}
	case EUnitType::CIWS:
	{
		SpawnedActor = World->SpawnActor<AGCUnitCIWS>(AGCUnitCIWS::StaticClass(), SpawnTransform);
		if (SpawnedActor != nullptr)
		{
			AGCUnitCIWS* CIWS = Cast<AGCUnitCIWS>(SpawnedActor);
			if (CIWS != nullptr) CIWS->BindActorManager(this);
			else GCLOG(Error, TEXT("Can't Cast CIWS"));
		}
		break;
	}
	case EUnitType::SEARAM:
	{
		SpawnedActor = World->SpawnActor<AGCUnitRAM>(AGCUnitRAM::StaticClass(), SpawnTransform);
		if (SpawnedActor != nullptr)
		{
			AGCUnitRAM* RAM = Cast<AGCUnitRAM>(SpawnedActor);
			if (RAM != nullptr) RAM->BindActorManager(this);
			else GCLOG(Error, TEXT("Can't Cast RAM"));
		}
		break;
	}
	case EUnitType::MARLIN:
	{
		SpawnedActor = World->SpawnActor<AGCUnitMarlin>(AGCUnitMarlin::StaticClass(), SpawnTransform);
		if (SpawnedActor != nullptr)
		{
			AGCUnitMarlin* Marlin = Cast<AGCUnitMarlin>(SpawnedActor);
			if (Marlin != nullptr) Marlin->BindActorManager(this);
			else GCLOG(Error, TEXT("Can't Cast Marlin"));
		}
		break;
	}
	case EUnitType::GATECLOSER:
	{
		SpawnedActor = World->SpawnActor<AGCUnitGateCloser>(AGCUnitGateCloser::StaticClass(), SpawnTransform);
		if (SpawnedActor != nullptr)
		{
			AGCUnitGateCloser* GCUnit = Cast<AGCUnitGateCloser>(SpawnedActor);
			if (GCUnit != nullptr) GCUnit->BindActorManager(this);
			else GCLOG(Error, TEXT("Can't Cast GateCloser"));

		}
		else GCLOG(Error, TEXT("Can't Spawn GateCloser"));
		break;
	}
	}
	
	return SpawnedActor;
}

void AGCGameActorManager::SetActorSpawnBoxDisable(AActor* InputActor)
{
	InputActor->GetComponentByClass<UStaticMeshComponent>()->DestroyComponent();
	OnActorDeployedDelegate.Broadcast();
}

bool AGCGameActorManager::GetActorCanBeDeploy(AActor* Actor, EUnitType InputAItype)
{
	bool Result = false;
	
	AGCUnitBase* TempActor = Cast<AGCUnitBase>(Actor);
	if (Actor != nullptr)
	{
		Result = TempActor->GetCanBeDeploy();
	}
	else
	{
		GCLOG(Log, TEXT("TempActor is nullptr"));
	}
		
	return Result;
}

void AGCGameActorManager::TimerTest()
{
	auto GameState = Cast<AGCGameState>(GetWorld()->GetGameState());
	if (FoundEnemies.Num() < 400) 
	{
		for (int32 i = 0; i < 25*GameState->GetEnemySpawnScale(); i++)
		{
			FVector Scale(1.0f, 1.0f, 1.0f);
			FTransform SpawnTransform;

			float Angle = FMath::RandRange(0.0f, 2.0f * PI); 

			FVector2D Rand = FVector2D(FMath::Cos(Angle), FMath::Sin(Angle)) * (FMath::RandRange(35000, 38200));
			
			SpawnTransform.SetLocation(FVector(Rand.X, Rand.Y, 25.0f));
			SpawnTransform.SetRotation(FQuat(FRotator(0.f, 0.f, 0.f)));
			SpawnTransform.SetScale3D(Scale);
			World->SpawnActor<AGCEnemy>(AGCEnemy::StaticClass(), SpawnTransform);
		}
	}
	SortEnemyByDist();

}

AActor* AGCGameActorManager::GetEnemyRandom()
{
	SortEnemyByDist();
	if (FoundEnemies.Num() > 0) 
	{
		AActor* Result = FoundEnemies[FMath::RandRange(0, FoundEnemies.Num() - 1)];
		return Result;
	}
	return nullptr;
}

AActor* AGCGameActorManager::GetEnemyClosest()
{
	SortEnemyByDist();
	if (FoundEnemies.Num() > 0)
	{
		AActor* Result = FoundEnemies[0];
		return Result;
	}
	return nullptr;
}

AActor* AGCGameActorManager::GetEnemyByMinDistance(FVector FiredLocation, float MinDistance)
{
	SortEnemyByDist();
	TArray<AActor*> FilteredActors;
	for (AActor* TmpActor : FoundEnemies) 
	{

		FVector Location = TmpActor->GetActorLocation();
		float Distance = FVector::Dist(Location, FiredLocation);
		if (Distance >= MinDistance) FilteredActors.Add(TmpActor);
		
	}
	if (FilteredActors.Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, FilteredActors.Num() - 1);
		return FilteredActors[RandomIndex];
	}
	return nullptr;
}

TArray<class AGCEnemy*> AGCGameActorManager::GetEnemyByClosestDistance(FVector CurrentLocation, float MaxDistance)
{
	SortEnemyByDist();
	TArray<class AGCEnemy*> FilteredEnemies;
	for (AActor* TmpActor : FoundEnemies)
	{

		FVector Location = TmpActor->GetActorLocation();
		float Distance = FVector::Dist(Location, CurrentLocation);
		if (Distance <= MaxDistance) 
		{
			AGCEnemy* TmpEnemy = Cast<AGCEnemy>(TmpActor);
			if(TmpEnemy != nullptr)\
				FilteredEnemies.Add(TmpEnemy);
		}
	}
	if (FilteredEnemies.Num() > 0) 
	{
		FilteredEnemies.Sort([&](const AActor& A, const AActor& B)
			{
				float DistanceA = FVector::Dist(A.GetActorLocation(), CurrentLocation);
				float DistanceB = FVector::Dist(B.GetActorLocation(), CurrentLocation);
				return DistanceA < DistanceB;
			});
		return FilteredEnemies;
	}
		
	

	return TArray<class AGCEnemy*>();
}



void AGCGameActorManager::SortEnemyByDist()
{
	UGameplayStatics::GetAllActorsOfClass(World, AGCEnemy::StaticClass(), FoundEnemies);
	if (FoundEnemies.Num() == 0) return;
	FoundEnemies.Sort([&](const AActor& A, const AActor& B)
		{
			float DistanceA = FVector::Dist(A.GetActorLocation(), FVector(0, 0, A.GetActorLocation().Z));
			float DistanceB = FVector::Dist(B.GetActorLocation(), FVector(0, 0, B.GetActorLocation().Z));
			return DistanceA < DistanceB;
		});
}

// Called when the game starts or when spawned
void AGCGameActorManager::BeginPlay()
{
	Super::BeginPlay();
	World = GetWorld();

	FTimerHandle MemberTimerHandle;
	
	World->GetTimerManager().SetTimer(MemberTimerHandle, this, &AGCGameActorManager::TimerTest, 15.0f, true);
	TimerTest();
}

// Called every frame
void AGCGameActorManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


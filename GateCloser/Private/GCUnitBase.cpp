// Fill out your copyright notice in the Description page of Project Settings.


#include "GCUnitBase.h"
#include "GCUnitGateCloser.h"
#include "GCGameActorManager.h"
#include "GCAllyAnimInstance.h"
#include "GCUnitBuffComponent.h"
#include "GCEnemy.h"
#include "GCGameState.h"
// Sets default values
AGCUnitBase::AGCUnitBase()
{
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("BOX"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MESH"));
	SpawnMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SPAWNMESH"));
	BuffComponent = CreateDefaultSubobject<UGCUnitBuffComponent>(TEXT("BUFFS"));
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	


	RootComponent = Box;
	Mesh->SetupAttachment(Box);
	SpawnMesh->SetupAttachment(Box);
	AudioComponent->SetupAttachment(RootComponent);

	SpawnMesh->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -100.f), FRotator::ZeroRotator);
	SpawnMesh->SetRelativeScale3D(FVector(4.f, 2.f, 2.f));
	SpawnMesh->GetPlacementExtent();

	Box->SetBoxExtent(FVector(99.0f, 99.0f, 50.0f));
	
	
	Mesh->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -100.f), FRotator::ZeroRotator);
	Mesh->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));

 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_SPAWN(TEXT("/Script/Engine.StaticMesh'/Game/StaticMeshes/SpawnBox.SpawnBox'"));
	if (SM_SPAWN.Succeeded())
	{
		SpawnMesh->SetStaticMesh(SM_SPAWN.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MI_EMPTY(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Materials/M_Empty_Inst.M_Empty_Inst'"));
	if (MI_EMPTY.Succeeded())
	{
		EmptyMaterial = MI_EMPTY.Object;
	}
	GCCHECK(EmptyMaterial != nullptr);

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MI_SA(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Materials/M_SpawnBox_Inst.M_SpawnBox_Inst'"));
	if (MI_SA.Succeeded())
	{
		SpawnAbleMaterial = MI_SA.Object;
	}
	GCCHECK(SpawnAbleMaterial != nullptr);

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MI_SE(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Materials/MI_SpawnBox_Cant.MI_SpawnBox_Cant'"));
	if (MI_SE.Succeeded())
	{
		SpawnEnableMaterial = MI_SE.Object;
	}
	Box->SetCollisionProfileName(TEXT("Ally"));
	SpawnMesh->SetCollisionProfileName(TEXT("NoCollision"));
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
}

// Called when the game starts or when spawned
void AGCUnitBase::BeginPlay()
{
	Super::BeginPlay();
	bCanBeDeploy = true;
	GameState = Cast<AGCGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (!GameState->IsPossibleToBuy())
	{
		SpawnMesh->SetMaterial(0, SpawnEnableMaterial);
		bCanBeDeploy = false;
	}
	GameState->OnCreditChanged.AddUObject(this, &AGCUnitBase::ToggleSpawnMesh);
}

void AGCUnitBase::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherbodyIndex, bool bFromSwepp, const FHitResult& SweepResult)
{
	if (SpawnMesh != nullptr)
	{
		SpawnMesh->SetMaterial(0, SpawnEnableMaterial);
		bCanBeDeploy = false;
	}
}

void AGCUnitBase::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherbodyIndex)
{
	TArray<AActor*> Result;
	TArray<AActor*> Result2;
	Box->GetOverlappingActors(Result, AGCUnitBase::StaticClass());
	Box->GetOverlappingActors(Result2, AGCUnitGateCloser::StaticClass());

	if ((SpawnMesh != nullptr) && !GameState->IsPossibleToBuy())
	{
		SpawnMesh->SetMaterial(0, SpawnEnableMaterial);
		bCanBeDeploy = false;
		return;
	}

	if ((SpawnMesh != nullptr) && ((Result.Num() + Result2.Num()) == 0))
	{
		SpawnMesh->SetMaterial(0, SpawnAbleMaterial);
		bCanBeDeploy = true;
	}

}

void AGCUnitBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Box->OnComponentBeginOverlap.AddDynamic(this, &AGCUnitBase::OnBoxBeginOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &AGCUnitBase::OnBoxEndOverlap);
}

// Called every frame
void AGCUnitBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGCUnitBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool AGCUnitBase::GetCanBeDeploy()
{
	return bCanBeDeploy;
}

void AGCUnitBase::SetCanAttack(bool NewBooleen)
{
	bCanAttack = NewBooleen;
}

FRotator AGCUnitBase::GetTargetRotate(TWeakObjectPtr<AGCEnemy> NewTargetActor)
{
	FVector CurrentLocation = PitchPos == FVector::ZeroVector ? GetActorLocation() : PitchPos;
	FVector TargetLocation = NewTargetActor->GetActorLocation();
	FVector DirectionVector = (TargetLocation - CurrentLocation).GetSafeNormal();
	FRotator TargetRotation = DirectionVector.Rotation();
	TargetRotation.Yaw = TargetRotation.Yaw + GetActorRotation().Yaw;
	return TargetRotation;
}

void AGCUnitBase::BindActorManager(AGCGameActorManager* NewActorManager)
{
	ActorManager = NewActorManager;
}

void AGCUnitBase::SetAnimDeployed()
{
	if (GCAnimInstance != nullptr) {
		GCAnimInstance->SetDeployed();
	}
}

void AGCUnitBase::ToggleSpawnMesh()
{
	if ((GameState == nullptr) || (SpawnMesh != nullptr))
	{
		return;
	}

	if ((SpawnMesh != nullptr) && this->GameState->IsPossibleToBuy())
	{
		this->SpawnMesh->SetMaterial(0, SpawnAbleMaterial);
		this->bCanBeDeploy = true;
	}
	else if ((SpawnMesh != nullptr) && !this->GameState->IsPossibleToBuy())
	{
		this->SpawnMesh->SetMaterial(0, SpawnEnableMaterial);
		this->bCanBeDeploy = false;
	}
}


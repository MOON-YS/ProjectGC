// Fill out your copyright notice in the Description page of Project Settings.


#include "GCEnemy.h"
#include "GCHPBar.h"
#include "GCUnitGateCloser.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GCGameState.h"
#include "Components/WidgetComponent.h"
#include "GCGameState.h"
// Sets default values
AGCEnemy::AGCEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("BOX"));

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MESH"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("STATICMESH"));
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));

	MaxHP = 100.f;
	CurrentHP = 100.f;

	RootComponent = Box;
	Mesh->SetupAttachment(Box);
	StaticMeshComponent->SetupAttachment(Box);
	HPBarWidget->SetupAttachment(RootComponent);

	Box->SetBoxExtent(FVector(25.0f, 25.0f, 25.0f));
	Box->SetRelativeLocation(FVector(0.0f, 0.f, 25.0f));
	Mesh->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -50.f), FRotator(0.f, -90.f, 0.f));
	Mesh->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));

	StaticMeshComponent->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -50.f), FRotator(0.f, -90.f, 0.f));
	StaticMeshComponent->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MESH(TEXT("/Script/Engine.SkeletalMesh'/Game/Enemy/01/DroneLeg.DroneLeg'"));
	if (SK_MESH.Succeeded())
	{
		SkeletalMesh_0 = SK_MESH.Object;
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> S_MESH1(TEXT("/Script/Engine.StaticMesh'/Game/Enemy/03/Evil_mech_01_FBX.Evil_mech_01_FBX'"));
	if (SK_MESH.Succeeded())
	{
		StaticMesh_01 = S_MESH1.Object;
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> S_MESH2(TEXT("/Script/Engine.StaticMesh'/Game/Enemy/02/Evil_mech_03_FBX.Evil_mech_03_FBX'"));
	if (SK_MESH.Succeeded())
	{
		StaticMesh_02 = S_MESH2.Object;
	}

	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/UI_HPBar.UI_HPBar_C'"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(30.0f, 10.0f));
	}
	
	NiagaraSystem = LoadObject<UNiagaraSystem>(nullptr, TEXT("/Script/Niagara.NiagaraSystem'/Game/AllExplosions/Niagara/NS_Air_1.NS_Air_1'"));

	
	Box->SetEnableGravity(false);
	Box->SetCollisionProfileName(TEXT("Enemy"));
}

// Called when the game starts or when spawned
void AGCEnemy::BeginPlay()
{
	Super::BeginPlay();
	bAlive = true;
	Box->OnComponentBeginOverlap.AddDynamic(this, &AGCEnemy::OnBoxBeginOverlap);
	auto EnemyWidget = Cast<UGCHPBar>(HPBarWidget->GetUserWidgetObject());
	

	float SpeedScale = 1.0f;

	auto GameState = Cast<AGCGameState>(GetWorld()->GetGameState());
	if (GameState != nullptr)
	{
		SpeedScale = GameState->GetEnemySpeedScale();
	}

	EnemyWidget->BindActor(this);

	int32 RollType = FMath::RandRange(0, 2);
	switch (RollType)
	{
	case 0:
	{
		ZAxis = 25.f;
		Mesh->SetSkeletalMesh(SkeletalMesh_0);
		StaticMeshComponent->DestroyComponent();
		Speed = 100.f* SpeedScale;
		break;
	}
	case 1:
	{
		Mesh->DestroyComponent();
		ZAxis = 300.f;
		StaticMeshComponent->SetStaticMesh(StaticMesh_01);
		StaticMeshComponent->SetRelativeLocation(FVector(0, 0, -100.f));
		Box->SetRelativeLocation(FVector(GetActorLocation().X, GetActorLocation().Y, ZAxis));
		Speed = 250 * SpeedScale;

		break;
	}
	case 2:
	{
		Mesh->DestroyComponent();
		ZAxis = 300.f;
		StaticMeshComponent->SetStaticMesh(StaticMesh_02);
		StaticMeshComponent->SetRelativeLocation(FVector(0, 0, -100.f));
		Box->SetRelativeLocation(FVector(GetActorLocation().X, GetActorLocation().Y, ZAxis));
		Speed = 200.f * SpeedScale;

		break;
	}
	}

	FVector TargetNormalVector = (FVector(0.f, 0.f, 0.f) - GetActorLocation()).GetSafeNormal();
	float DotProduct = FVector::DotProduct(TargetNormalVector, GetActorForwardVector().GetSafeNormal());
	float AngleRadians = FMath::Acos(DotProduct);
	float AngleDegrees = FMath::RadiansToDegrees(AngleRadians);
	if (TargetNormalVector.Y < 0 && GetActorRotation().Yaw == 0) {
		AngleDegrees = -AngleDegrees;
	}
	Box->SetRelativeRotation(FRotator(0.0f, AngleDegrees, 0.0f));

	MoveDirectionNormal = (FVector(0.f, 0.f, ZAxis) - GetActorLocation()).GetSafeNormal();
}

void AGCEnemy::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherbodyIndex, bool bFromSwepp, const FHitResult& SweepResult)
{
	TArray<AActor*> Result;
	Box->GetOverlappingActors(Result, AGCUnitGateCloser::StaticClass());
	if (Result.Num() == 1) 
	{
		AGCUnitGateCloser* UnitGC = Cast<AGCUnitGateCloser>(Result[0]);
		UnitGC->SetDamage(10);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraSystem, GetActorLocation(), GetActorRotation());
		Destroy();
	}
}

float AGCEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (ActualDamage > 0.f)
	{
		CurrentHP -= ActualDamage; // 플레이어 체력 감소
		if (CurrentHP <= 0.f)
		{
			Destroy(); // 플레이어 사망 처리
		}

	}
	return ActualDamage;
}

// Called every frame
void AGCEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation((MoveDirectionNormal * (DeltaTime) * Speed) + GetActorLocation());

}

void AGCEnemy::BindActorManager(AGCGameActorManager* NewActorManager)
{
	ActorManager = NewActorManager;
}

float AGCEnemy::GetHPRatio()
{
	return (MaxHP < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentHP / MaxHP);
}

void AGCEnemy::SetDamage(float AttackPoint)
{
	CurrentHP -= AttackPoint;
	OnHPChanged.Broadcast();
	if ((CurrentHP < KINDA_SMALL_NUMBER) && bAlive)
	{
		AGCGameState* GameState = Cast<AGCGameState>(UGameplayStatics::GetGameState(GetWorld()));
		GameState->AddExp(40);
		GameState->AddCredit(10);
		CurrentHP = 0.0f;
		Speed = 0.0f;
		bAlive = false;
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraSystem, GetActorLocation(), GetActorRotation());
		Destroy();
	}
}

FVector AGCEnemy::GetDirection()
{
	return MoveDirectionNormal;
}

float AGCEnemy::GetSpeed()
{
	return Speed;
}


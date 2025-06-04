// Fill out your copyright notice in the Description page of Project Settings.


#include "GCPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
#include "Engine/StaticMeshActor.h"
#include "EngineUtils.h"
#include "GCGameActorManager.h"
#include "NiagaraActor.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "GCSpawnHUDWidget.h"
#include "GCSelectModeWidget.h"
#include "GCUnitBase.h"
#include "GCUnitGateCloser.h"
#include "GCPlayerState.h"
#include "GCMainHud.h"
#include "GCGameState.h"
#include "GCRandomBuffSelect.h"
#include "GCUIBuffs.h"
#include "Blueprint/UserWidget.h"
AGCPlayerController::AGCPlayerController()
{
	static ConstructorHelpers::FClassFinder<UGCSpawnHUDWidget> UI_HUDSPAWN_C(TEXT("/Game/UI/UI_SpawnHUD.UI_SpawnHUD_C"));
	if (UI_HUDSPAWN_C.Succeeded())
	{
		SpawnHUDClass = UI_HUDSPAWN_C.Class;
	}

	static ConstructorHelpers::FClassFinder<UGCSelectModeWidget> UI_SELECT_C(TEXT("/Game/UI/UI_SelectModeDial.UI_SelectModeDial_C"));
	if (UI_SELECT_C.Succeeded())
	{
		SelectHUDClass = UI_SELECT_C.Class;
	}
	static ConstructorHelpers::FClassFinder<UGCMainHud> UI_MAIN_C(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/UI_HUD.UI_HUD_C'"));
	if (UI_MAIN_C.Succeeded())
	{
		MainHudClass = UI_MAIN_C.Class;
	}
	static ConstructorHelpers::FClassFinder<UGCUIBuffs> UI_BUFF_C(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/UI_Status.UI_Status_C'"));
	if (UI_BUFF_C.Succeeded())
	{
		BuffStatClass = UI_BUFF_C.Class;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_UI(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_UI.IA_UI'"));
	if (IA_UI.Succeeded())
	{
		UIAction = IA_UI.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_BUILD(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Build.IA_Build'"));
	if (IA_BUILD.Succeeded())
	{
		BuildAction = IA_BUILD.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_CANCLEBUILD(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_CancleBuild.IA_CancleBuild'"));
	if (IA_CANCLEBUILD.Succeeded())
	{
		CancleBuildAction = IA_CANCLEBUILD.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_ROTATE(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Build_Rotate.IA_Build_Rotate'"));
	if (IA_ROTATE.Succeeded())
	{
		RotateAction = IA_ROTATE.Object;
	}
	static ConstructorHelpers::FClassFinder<UGCRandomBuffSelect> UI_SELECTBUFF_C(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/UI_RandomBuffSelect.UI_RandomBuffSelect_C'"));
	if (UI_SELECTBUFF_C.Succeeded())
	{
		BuffSelectClass = UI_SELECTBUFF_C.Class;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_PAUSE(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Pause.IA_Pause'"));
	if (IA_PAUSE.Succeeded())
	{
		PauseAction = IA_PAUSE.Object;
	}
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_PAUSE_C(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/UI_Pause.UI_Pause_C'"));
	if (UI_PAUSE_C.Succeeded())
	{
		PauseUIClass = UI_PAUSE_C.Class;
	}
}

void AGCPlayerController::SetCurrentSpawnActor(AActor* NewActor)
{
	CurrentSpawnActor = NewActor;
}

void AGCPlayerController::MouseLbClicked()
{
	switch (GCPlayerState->GetPlayerCurrentControlMode())
	{
	case EPlayerControlMode::BUILD:
	{
		if (CurrentSpawnActor == nullptr) {
			return; 
		}
		FHitResult Hit;
		bool bHitSuccessful = false;
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel4, true, Hit);
		FVector CopyLocation = Hit.Location;
		CopyLocation.Z = 0.0f;
		if (ActorManager->GetActorCanBeDeploy(CurrentSpawnActor, CurrentBuildType))
		{
			ActorManager->SetActorSpawnBoxDisable(CurrentSpawnActor);
			AGCUnitBase* CastedUnit = Cast<AGCUnitBase>(CurrentSpawnActor);
			if (CastedUnit != nullptr) CastedUnit->SetAnimDeployed();
			AGCGameState* GameState = Cast<AGCGameState>(UGameplayStatics::GetGameState(GetWorld()));
			GameState->Purchase();

			CurrentSpawnActor = ActorManager->SpawnAI(CopyLocation, BuildRotate,CurrentBuildType);

			SetCurrentActorNullptr();
			GCPlayerState->SetPlayerCurrentControlMode(EPlayerControlMode::VIEW);
			bShowMouseCursor = true;
			

		}
		break;
	}
	case EPlayerControlMode::VIEW:
		break;
	}


}



void AGCPlayerController::MouseRbClicked()
{	
	
	if (GCPlayerState->GetPlayerCurrentControlMode() == EPlayerControlMode::BUILD)
	{
		SetCurrentActorNullptr();
	}
	bShowMouseCursor = true;
	SpawnHUD->SetCursor(EMouseCursor::Default);
}

void AGCPlayerController::BeginPlay()
{
	Super::BeginPlay();
	GCPlayerState = Cast<AGCPlayerState>(PlayerState);
	bShowMouseCursor = true;

	EMouseLockMode::LockInFullscreen;
	BuffsStatUI = CreateWidget<UGCUIBuffs>(this, BuffStatClass);
	BuffsStatUI->AddToViewport(3);
	BuffsStatUI->SetVisibility(ESlateVisibility::Hidden);

	PauseUI = CreateWidget<UUserWidget>(this, PauseUIClass);
	PauseUI->AddToViewport(5);
	PauseUI->SetVisibility(ESlateVisibility::Hidden);

	SpawnHUD = CreateWidget<UGCSpawnHUDWidget>(this, SpawnHUDClass);
	SpawnHUD->AddToViewport(1);
	SpawnHUD->BindPlayerState(GCPlayerState);
	SpawnHUD->BindPlayerController(this);
	SpawnHUD->BindActorManager(ActorManager);

	SelectHUD = CreateWidget<UGCSelectModeWidget>(this, SelectHUDClass);
	SelectHUD->BindPlayerState(GCPlayerState);
	SelectHUD->BindPlayerController(this);
	SelectHUD->AddToViewport(1);
	SelectHUD->SetVisibility(ESlateVisibility::Hidden);
	GCPlayerState->OnControlModeChanged.AddUObject(this, &AGCPlayerController::SetCurrentActorNullptr);
	SelectHUD->BindBuffsWidget(BuffsStatUI);


	MainHud = CreateWidget<UGCMainHud>(this, MainHudClass);
	MainHud->AddToViewport(0);
	MainHud->BindGameState();
	MainHud->UpdateState();
	BuffSelect = CreateWidget<UGCRandomBuffSelect>(this, BuffSelectClass);
	BuffSelect->AddToViewport(2);
	BuffSelect->SetVisibility(ESlateVisibility::Hidden);
	BuffSelect->SetGameState();
	AGCGameState* GameState = Cast<AGCGameState>(UGameplayStatics::GetGameState(GetWorld()));
	GameState->OnLevelChanged.AddLambda([this]() {
		BuffSelect->SetVisibility(ESlateVisibility::Visible);
		});
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	FTransform BasePos;
	BasePos.SetLocation(FVector(0.f, 0.f, 200.f));
	BasePos.SetRotation(FQuat(FRotator::ZeroRotator));
	BasePos.SetScale3D(FVector(1.f, 1.f, 1.f));

	AActor* Base = GetWorld()->SpawnActor<AGCUnitGateCloser>(AGCUnitGateCloser::StaticClass(), BasePos);
	AGCUnitGateCloser* BaseUnit = Cast<AGCUnitGateCloser>(Base);
	BaseUnit->BindActorManager(ActorManager);
	
}

void AGCPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (UEnhancedInputComponent* PlayerInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		PlayerInputComponent->BindAction(UIAction, ETriggerEvent::Started, this, &AGCPlayerController::ShowModeSelectHUD);
		PlayerInputComponent->BindAction(UIAction, ETriggerEvent::Completed, this, &AGCPlayerController::HideModeSelectHUD);
		PlayerInputComponent->BindAction(BuildAction, ETriggerEvent::Completed, this, &AGCPlayerController::MouseLbClicked);
		PlayerInputComponent->BindAction(CancleBuildAction, ETriggerEvent::Completed, this, &AGCPlayerController::MouseRbClicked);
		PlayerInputComponent->BindAction(RotateAction, ETriggerEvent::Completed, this, &AGCPlayerController::ToggleBuildRotate);
		PlayerInputComponent->BindAction(PauseAction, ETriggerEvent::Completed, this, &AGCPlayerController::PauseGame);
	}
	
}

void AGCPlayerController::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGCGameActorManager::StaticClass(),FoundActors);

	ActorManager = GetWorld()->SpawnActor<AGCGameActorManager>(AGCGameActorManager::StaticClass(), FVector::ZeroVector,FRotator::ZeroRotator);

	FoundActors.Empty();

}

void AGCPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if ((GCPlayerState->GetPlayerCurrentControlMode() == EPlayerControlMode::BUILD) && (CurrentSpawnActor != nullptr))
	{
		FHitResult Hit;
		bool bHitSuccessful = false;
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel4, true, Hit);
		if (bHitSuccessful)
		{
			double Factor = 100.0f;
			FVector Rounded = FVector(FMath::RoundToDouble(Hit.Location.X / Factor)* Factor, FMath::RoundToDouble(Hit.Location.Y / Factor)* Factor, 100.0f);
			CurrentSpawnActor->SetActorLocation(Rounded);
		}
	}

}

void AGCPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	
}

void AGCPlayerController::ShowModeSelectHUD()
{

	FInputModeUIOnly InputMode;
	SetInputMode(InputMode);
	if (SelectHUD->GetVisibility() != ESlateVisibility::Visible)
	{
		SelectHUD->SetFocus();
		SelectHUD->SetVisibility(ESlateVisibility::Visible);
		SelectHUD->SetCursor(EMouseCursor::Crosshairs);
	}

}

void AGCPlayerController::HideModeSelectHUD()
{
	//GCLOG(Log, TEXT("Released"));
	//if (SelectHUD->GetVisibility() != ESlateVisibility::Hidden)
	//{
	//	SelectHUD->SetVisibility(ESlateVisibility::Hidden);
	//}

}

void AGCPlayerController::PauseGame()
{

	FInputModeUIOnly InputMode;
	SetInputMode(InputMode);
	PauseUI->SetVisibility(ESlateVisibility::Visible);
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void AGCPlayerController::SetCurrentBuildType(EUnitType NewBuildType)
{
	
	CurrentBuildType = NewBuildType;
}


void AGCPlayerController::SwapBuildTypeOnCursor(EUnitType NewType)
{
	CurrentBuildType = NewType;
	if(CurrentSpawnActor!=nullptr) CurrentSpawnActor->Destroy();
	FHitResult Hit;
	bool bHitSuccessful = false;
	bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel4, true, Hit);
	GCCHECK(bHitSuccessful);
	FVector CopyLocation = Hit.Location;
	CopyLocation.Z = 100.0f;
	CurrentSpawnActor = ActorManager->SpawnAI(CopyLocation,BuildRotate,NewType);
}

void AGCPlayerController::SetCurrentActorNullptr()
{
	if ((CurrentSpawnActor != nullptr))
	{
		CurrentSpawnActor->Destroy();
		CurrentSpawnActor = nullptr;
	}
}

void AGCPlayerController::ToggleBuildRotate()
{
	if ((GCPlayerState->GetPlayerCurrentControlMode() == EPlayerControlMode::BUILD) && (CurrentSpawnActor != nullptr))
	{
		if (BuildRotate == FRotator::ZeroRotator)
		{
			BuildRotate = FRotator(0.f, 90.f, 0.f);
		}
		else
		{
			BuildRotate = FRotator::ZeroRotator;
		}
		CurrentSpawnActor->SetActorRotation(BuildRotate);
	}
}

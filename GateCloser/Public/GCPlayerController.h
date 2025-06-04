// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "GateCloser.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GCPlayerController.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnMovePosSetted)

UCLASS()
class GATECLOSER_API AGCPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	
	UInputAction* UIAction;

	UInputAction* BuildAction;

	UInputAction* CancleBuildAction;

	UInputAction* RotateAction;

	UInputAction* PauseAction;
public:
	AGCPlayerController();

	void SetCurrentSpawnActor(AActor* NewActor);
	void SwapBuildTypeOnCursor(EUnitType NewType);
	void SetCurrentActorNullptr();
	void ToggleBuildRotate();

	FOnMovePosSetted OnMovePosSetted;
private:
	void MouseRbClicked();
	void MouseLbClicked();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void PreInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnPossess(APawn* aPawn) override;
	void ShowModeSelectHUD();
	void HideModeSelectHUD();
	void PauseGame();

	void SetCurrentBuildType(EUnitType NewBuildType);
	

protected:
	class AGCGameActorManager* ActorManager;
	TArray<class AGCAICharacter*> SelectedActors;
	class ANiagaraActor* NiagaraActor;
	AActor* CurrentSpawnActor;
	EUnitType CurrentBuildType;
	class AGCPlayerState* GCPlayerState;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UGCSpawnHUDWidget> SpawnHUDClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UGCSelectModeWidget> SelectHUDClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UGCMainHud> MainHudClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UGCRandomBuffSelect> BuffSelectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UGCUIBuffs> BuffStatClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UUserWidget> PauseUIClass;


private:

	UPROPERTY()
	class UGCSpawnHUDWidget* SpawnHUD;

	UPROPERTY()
	class UGCSelectModeWidget* SelectHUD;

	UPROPERTY()
	class UGCMainHud* MainHud;

	UPROPERTY()
	class UGCRandomBuffSelect* BuffSelect;

	UPROPERTY()
	class UGCUIBuffs* BuffsStatUI;

	UPROPERTY()
	class UUserWidget* PauseUI;

	FRotator BuildRotate = FRotator::ZeroRotator;
};

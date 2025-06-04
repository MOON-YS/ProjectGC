// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GCHPBar.generated.h"

/**
 * 
 */
UCLASS()
class GATECLOSER_API UGCHPBar : public UUserWidget
{
	GENERATED_BODY()
public:
	void BindActor(class AGCEnemy* NewEnemyActor);
	
protected:
	void UpdateHPBar();

private:
	TWeakObjectPtr<class AGCEnemy> CurrentEnemyActor;

	UPROPERTY()
	class UProgressBar* HPProgressBar;
};

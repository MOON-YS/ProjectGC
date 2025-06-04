// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GCUIResult.generated.h"

/**
 * 
 */
UCLASS()
class GATECLOSER_API UGCUIResult : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void SetResultData(int32 TimeRemain,int32 Score);
private:
	UPROPERTY()
	class UTextBlock* Results;

	UPROPERTY()
	class UTextBlock* KillScore;

	UPROPERTY()
	class UTextBlock* SurviveTime;



};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GateCloser.h"
#include "Animation/AnimInstance.h"
#include "GCAllyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GATECLOSER_API UGCAllyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UGCAllyAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	void SetDeployed(bool IsDeployed=true);
	void SetTargetRotate(FRotator NewTargetRotate);
	bool GetDeployed() { return bDeployed; };
	void BindActor(class AGCUnitBase* NewAI, EUnitType Type);
	void SetReadyToFire() { bReadyToFire = true; };
	bool GetRotationEnded() { return bRotationEnd; };
	bool GetReadyToFire() { return bReadyToFire; };
	FRotator GetCurrentRotation() { return CurrentRotation; };

	UFUNCTION()
	void AnimNotify_Deployed();

	UFUNCTION()
	void AnimNotify_Fire();

	UFUNCTION()
	void AnimNotify_Fired();

	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	FRotator TargetRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	FRotator CurrentRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	bool bDeployed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	float TargetRotate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	float RotateSetted;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	bool bRotationEnd;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	bool bReadyToFire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	EUnitType UnitType;

	class AGCUnitBase* Unit;

};

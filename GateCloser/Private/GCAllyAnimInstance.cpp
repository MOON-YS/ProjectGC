// Fill out your copyright notice in the Description page of Project Settings.


#include "GCAllyAnimInstance.h"
#include "GCUnitBase.h"
#include "GCUnitPatriot.h"
#include "GCUnitGateCloser.h"
#include "GCUnitM777.h"
#include "GCUnitCIWS.h"
#include "GCUnitRAM.h"
#include "GCUnitM1A1.h"
#include "GCUnitMarlin.h"

UGCAllyAnimInstance::UGCAllyAnimInstance()
{
	bDeployed = false;
	TargetRotate = 0.f;
	bRotationEnd = false;
	bReadyToFire = false;
}

void UGCAllyAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
}

void UGCAllyAnimInstance::SetDeployed(bool IsDeployed)
{
	bDeployed = IsDeployed;
}

void UGCAllyAnimInstance::SetTargetRotate(FRotator NewTargetRotate)
{
	TargetRotation = NewTargetRotate;
}

void UGCAllyAnimInstance::BindActor(AGCUnitBase* NewUnit,EUnitType Type)
{
	Unit = NewUnit;
	UnitType = Type;
}

void UGCAllyAnimInstance::AnimNotify_Deployed()
{
	
	switch (UnitType)
	{
	case EUnitType::ENEMY:
		break;
	case EUnitType::ENEMY_BOSS:
		break;
	case EUnitType::PATRIOT:
	{
		AGCUnitPatriot* Patriot = Cast<AGCUnitPatriot>(Unit);
		Patriot->SetTimer();
		break;
	}
	case EUnitType::M777:
	{
		AGCUnitM777* M777 = Cast<AGCUnitM777>(Unit);
		M777->SetTimer();
		break;
	}
	case EUnitType::CIWS:
	{
		AGCUnitCIWS* CIWS = Cast<AGCUnitCIWS>(Unit);
		CIWS->SetTimer();
		break;
	}
	case EUnitType::SEARAM:
	{
		AGCUnitRAM* RAM = Cast<AGCUnitRAM>(Unit);
		RAM->SetTimer();
		break;
	}
	case EUnitType::M1A1:
	{
		AGCUnitM1A1* M1A1 = Cast<AGCUnitM1A1>(Unit);
		M1A1->SetTimer();
		break;
	}
	case EUnitType::MARLIN:
	{
		AGCUnitMarlin* Marlin = Cast<AGCUnitMarlin>(Unit);
		Marlin->SetTimer();
		break;
	}
	case EUnitType::GATECLOSER:
		break;
	case EUnitType::NONE:
		break;
	default:
		break;
	}
}

void UGCAllyAnimInstance::AnimNotify_Fire()
{
	switch (UnitType)
	{
	case EUnitType::ENEMY:
		break;
	case EUnitType::ENEMY_BOSS:
		break;
	case EUnitType::PATRIOT:
	{
		bReadyToFire = false;
		AGCUnitPatriot* Patriot = Cast<AGCUnitPatriot>(Unit);
		Patriot->Fire();
		break;
	}
	case EUnitType::M777:
	{
		bReadyToFire = false;
		AGCUnitM777* M777 = Cast<AGCUnitM777>(Unit);
		M777->Fire();
		break;
	}
	case EUnitType::M1A1:
	{
		bReadyToFire = false;
		AGCUnitM1A1* M1A1 = Cast<AGCUnitM1A1>(Unit);
		M1A1->Fire();
		break;
	}
	case EUnitType::GATECLOSER:
	{
	}
	case EUnitType::SEARAM:
	{
		bReadyToFire = false;
		AGCUnitRAM* RAM = Cast<AGCUnitRAM>(Unit);
		RAM->Fire();
		break;
	}
	case EUnitType::NONE:
		break;
	default:
		break;
	}
}

void UGCAllyAnimInstance::AnimNotify_Fired()
{
	switch (UnitType)
	{
	case EUnitType::ENEMY:
		break;
	case EUnitType::ENEMY_BOSS:
		break;
	case EUnitType::PATRIOT:
		break;
	case EUnitType::M777:
		break;
	case EUnitType::M1A1:
		break;
	case EUnitType::GATECLOSER:
		break;
	case EUnitType::NONE:
		break;
	default:
		break;
	}
}

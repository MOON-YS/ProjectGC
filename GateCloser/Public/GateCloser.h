// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"

UENUM(BlueprintType)
enum class EUnitType :uint8
{
	ENEMY,
	ENEMY_BOSS,
	PATRIOT,
	M777,
	M1A1,
	CIWS,
	SEARAM,
	MARLIN,
	GATECLOSER,
	NONE
};

UENUM(BlueprintType)
enum class EPlayerControlMode :uint8
{
	VIEW,
	BUILD
};

DECLARE_LOG_CATEGORY_EXTERN(GateCloser, Log, All);

#define GCLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define GCLOG_S(Verbosity) UE_LOG(GateCloser,Verbosity,TEXT("%s"),*GCLOG_CALLINFO)
#define GCLOG(Verbosity,Format, ...) UE_LOG(GateCloser, Verbosity, TEXT("%s %s"), *GCLOG_CALLINFO, *FString::Printf(Format,##__VA_ARGS__))
#define GCCHECK(Expr, ...) {if(!(Expr)){GCLOG(Error, TEXT("ASSERTION : %s"),TEXT("'"#Expr"'")); return __VA_ARGS__;}}
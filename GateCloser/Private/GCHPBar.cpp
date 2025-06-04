// Fill out your copyright notice in the Description page of Project Settings.


#include "GCHPBar.h"
#include "GCEnemy.h"
#include "Components/ProgressBar.h"
void UGCHPBar::BindActor(AGCEnemy* NewEnemyActor)
{
	CurrentEnemyActor = NewEnemyActor;

	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));
	CurrentEnemyActor->OnHPChanged.AddUObject(this, &UGCHPBar::UpdateHPBar);
	UpdateHPBar();
}

void UGCHPBar::UpdateHPBar()
{
	if (CurrentEnemyActor.IsValid())
	{
		if (nullptr != HPProgressBar)
		{
			HPProgressBar->SetPercent(CurrentEnemyActor->GetHPRatio());
		}
	}
}

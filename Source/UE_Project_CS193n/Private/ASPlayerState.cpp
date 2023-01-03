// Fill out your copyright notice in the Description page of Project Settings.


#include "ASPlayerState.h"

AASPlayerState::AASPlayerState()
{
	credits = 0;
}

void AASPlayerState::AddCredits(int32 Delta)
{
	if (Delta <= 0)
	{
		return;
	}
	credits += Delta;
	OnCreditsChanged.Broadcast(this, credits, Delta);
}

int32 AASPlayerState::GetCredits()
{
	return credits;
}

bool AASPlayerState::RemoveCredits(int32 Delta)
{
	if (credits < Delta)
	{
		return false;
	}
	if (Delta < 0)
	{
		return false;
	}
	credits -= Delta;

	OnCreditsChanged.Broadcast(this, credits, Delta);
	return true;
}

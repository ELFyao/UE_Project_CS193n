// Fill out your copyright notice in the Description page of Project Settings.


#include "SCreditsCoin.h"
#include "ASPlayerState.h"

ASCreditsCoin::ASCreditsCoin()
{
	CreditsBonusAmount = 1000;
}

bool ASCreditsCoin::PlayFunction(APawn* InstigatorPawn)
{
	AASPlayerState* playerState = InstigatorPawn->GetPlayerState<AASPlayerState>();
	ensureAlways(playerState);
	if (playerState) {
		playerState->AddCredits(CreditsBonusAmount);
		return true;
	}
	return false;
}

void ASCreditsCoin::CooldownOver()
{
	bIsCooldown = true;
	Destroy();
}

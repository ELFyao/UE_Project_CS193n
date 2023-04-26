// Fill out your copyright notice in the Description page of Project Settings.


#include "SCreditsCoin.h"
#include "ASPlayerState.h"
#include "Components/SphereComponent.h"
ASCreditsCoin::ASCreditsCoin()
{
	CreditsBonusAmount = 1000;
	//SetReplicates(true);
	//SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");

	//RootComponent = SphereComp;
	//BaseMesh->SetupAttachment(RootComponent);
	bReplicates = true;
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

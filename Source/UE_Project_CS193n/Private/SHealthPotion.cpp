// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"
#include "SAttributeComponent.h"
#include "ASPlayerState.h"

ASHealthPotion::ASHealthPotion()
{
	HealVolume = 125.0f;
	CreditsCost = 500;
}



bool ASHealthPotion::PlayFunction(APawn* InstigatorPawn)
{
	if (InstigatorPawn) {
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
		AASPlayerState *playerstate = InstigatorPawn->GetPlayerState<AASPlayerState>();
		
		ensureAlways(playerstate);

		if ( !playerstate->RemoveCredits(CreditsCost)) {
			
			FString DebugMsg = FString::Printf(TEXT("Healing Failed : required at least %d"), CreditsCost);
			GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, DebugMsg);

			return false;
		}
		if (AttributeComp) {
			if (AttributeComp->GetIsFull() == false)
			{
				AttributeComp->ApplyHealthChange(InstigatorPawn, HealVolume);
				return true;
			}
		}
	}
	return false;
}

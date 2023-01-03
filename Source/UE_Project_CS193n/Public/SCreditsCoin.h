// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SItemBase.h"
#include "SCreditsCoin.generated.h"

/**
 * 
 */
UCLASS()
class UE_PROJECT_CS193N_API ASCreditsCoin : public ASItemBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "CreditsProperty")
	float CreditsBonusAmount;

public:
	ASCreditsCoin();

	virtual bool PlayFunction(APawn* InstigatorPawn) override;

	virtual void CooldownOver() override;
};

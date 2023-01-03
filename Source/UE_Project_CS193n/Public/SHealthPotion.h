// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SItemBase.h"
#include "SHealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class UE_PROJECT_CS193N_API ASHealthPotion : public ASItemBase
{
	GENERATED_BODY()
protected:

	UPROPERTY(EditDefaultsOnly, Category = "HealthProperty")
	float HealVolume;

	UPROPERTY(EditDefaultsOnly, Category = "HealthCost")
	int32 CreditsCost;
public:


	ASHealthPotion();


	virtual bool PlayFunction(APawn* InstigatorPawn) override;

};

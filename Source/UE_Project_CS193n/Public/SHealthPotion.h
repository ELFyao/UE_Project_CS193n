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


public:
	UPROPERTY(EditAnywhere, Category = "HealthProperty")
	float HealVolume;


	ASHealthPotion();


	bool bIsCooldown;
	void CooldownOver();
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	virtual void Pick_Implementation() override;
};

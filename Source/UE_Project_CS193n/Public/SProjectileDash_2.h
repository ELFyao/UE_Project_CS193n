// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileParent.h"
#include "SProjectileDash_2.generated.h"

/**
 * 
 */
UCLASS()
class UE_PROJECT_CS193N_API ASProjectileDash_2 : public ASProjectileParent
{
	GENERATED_BODY()
public:
	ASProjectileDash_2();
	void ProjectileAttack_Implementation(APawn* InstigatorPawn) override;

	void ProjectileEffect(APawn* InstigatorPawn) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every framer
	virtual void Tick(float DeltaTime) override;
	/*virtual void ProjectileEffect(APawn* InstigatorPawn)override;*/
	/*void ProjectileAttack_Implementation(APawn* InstigatorPawn) override;*/
};

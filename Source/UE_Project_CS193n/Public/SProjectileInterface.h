// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SProjectileInterface.generated.h"

// This class does not need to be modified.

//class USphereComponent;
//class UProjectileMovementComponent;
//class UParticleSystemComponent;

UINTERFACE(MinimalAPI)
class USProjectileInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UE_PROJECT_CS193N_API ISProjectileInterface
{
	GENERATED_BODY()

public:
	//UPROPERTY(VisibleAnywhere)
	//float AttackSpeed;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//USphereComponent* SphereComp;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//UProjectileMovementComponent* MovementComp;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//UParticleSystemComponent* EffectComp;
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ProjectileAttack(APawn* InstigatorPawn);
};

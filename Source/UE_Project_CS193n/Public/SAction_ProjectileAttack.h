// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SAction_ProjectileAttack.generated.h"

/**
 * 
 */
UCLASS()
class UE_PROJECT_CS193N_API USAction_ProjectileAttack : public USAction
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HandSocketName;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParams;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackDistance;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackAnimDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* HandFlashVFX;

	UFUNCTION()
	void AttackDelayElapsed(ACharacter* InstigatorActor);

public:
	virtual void StartAction_Implementation(AActor* InstigatorActor) override;

	USAction_ProjectileAttack();

	UWorld* GetWorld() const override;
};
 
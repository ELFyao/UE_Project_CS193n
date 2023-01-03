// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SItemBase.generated.h"


class UStaticMeshComponent;


UCLASS()
class UE_PROJECT_CS193N_API ASItemBase : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASItemBase();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "HealthProperty")
	float CooldownSeconds;


	bool bIsCooldown;
public:	

	UFUNCTION()
	virtual void CooldownOver();

	UFUNCTION()
	virtual bool PlayFunction(APawn* InstigatorPawn);

	FTimerHandle CooldownTimer;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* BaseMesh;


	void Interact_Implementation(APawn* InstigatorPawn);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SBTService_CheckLowHealth.generated.h"

/**
 * 
 */
UCLASS()
class UE_PROJECT_CS193N_API USBTService_CheckLowHealth : public UBTService
{
	GENERATED_BODY()


public:
	USBTService_CheckLowHealth();
protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector LowHealthKey;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	float LowHealthFraction;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
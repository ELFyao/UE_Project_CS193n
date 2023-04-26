// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, USAttributeComponent*, OwingComp, float, NewHealth, float, Dealta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_PROJECT_CS193N_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USAttributeComponent();

	static USAttributeComponent* GetAttributes(AActor *FromActor);
	static bool IsActorAlive(AActor* FromActor);


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float LowHealthThreshold;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Replicated, Category = "Attributes")
	float Health;
	// Called when the game starts
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated,  Category = "Attributes")
	float HealthMax;


	//UPROPERTY(ReplicatedUsing = "OnRep_Dead")
	//bool bIsAlive;

	//UFUNCTION(Reliable)
	//void OnRep_Dead();

	UFUNCTION(NetMulticast, Reliable) //@fixme change to unreliable 
	void MulticastHealthChanged(AActor* InstigatorActor, float NewHealth, float Delta);
public:
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	UFUNCTION(BlueprintCallable)
	bool GetIsAlive();

	bool GetIsLowHealth();

	bool HealMax();

	float GetHealth();

	float GetMaxHitPoint();

	UFUNCTION(BlueprintCallable)
	bool GetIsFull();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	bool Kill(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);
};

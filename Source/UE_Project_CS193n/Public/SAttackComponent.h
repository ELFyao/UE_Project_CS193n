// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttackComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_PROJECT_CS193N_API USAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	void ProjectileDash(); 
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> DashProjectileClass;

	FTimerHandle TimerHandle_PrimaryAttack;
public:	
	// Sets default values for this component's properties
	USAttackComponent();



public:
	void SpawnDashProjectile(FTransform SpawnTM, FActorSpawnParameters SpawnParams);
	void SpawnProjectile(TSubclassOf<AActor> Projectile, FTransform SpawnTM, FActorSpawnParameters SpawnParams);

protected:


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


		
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectileInterface.h"
#include "SProjectileParent.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;


UCLASS()
class UE_PROJECT_CS193N_API ASProjectileParent : public AActor, public ISProjectileInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASProjectileParent();
	UPROPERTY(VisibleAnywhere)
	float AttackSpeed;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* MovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* EffectComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void ProjectileEffect(APawn* InstigatorPawn) {};

public:
	void ProjectileAttack_Implementation(APawn* InstigatorPawn) override;
};

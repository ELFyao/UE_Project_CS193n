// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SProjectileInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SDashProjectile.generated.h"
class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;
class UParticleSystem;

UCLASS()
class UE_PROJECT_CS193N_API ASDashProjectile : public AActor, public ISProjectileInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASDashProjectile();
	UPROPERTY(VisibleAnywhere)
	float AttackSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* MovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* EffectComp;
	// Add interface functions to this class. T

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UParticleSystem * EmitterComp;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PostInitializeComponents() override;


	FVector DashLocation;
	FTimerHandle TimerHandle_PrimaryDash;
	FTimerHandle TimerHandle_PrimaryDashWaiting;

	void PrimaryDash();
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	/*void ProjectileAttack_Implementation(APawn* InstigatorPawn) override;*/


	void ProjectileAttack_Implementation(APawn* InstigatorPawn) override;

};

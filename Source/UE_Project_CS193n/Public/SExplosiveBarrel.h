// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SExplosiveBarrel.generated.h"


class UStaticMeshComponent;
class URadialForceComponent;



UCLASS()
class UE_PROJECT_CS193N_API ASExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASExplosiveBarrel();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent *StaticMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	URadialForceComponent *RadialForceComp;


	UFUNCTION()
	void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
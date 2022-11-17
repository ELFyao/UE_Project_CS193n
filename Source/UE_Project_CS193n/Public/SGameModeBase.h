// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "SGameModeBase.generated.h"



class UEnvQuery;
class UCurveFloat;
class UEnvQueryInstanceBlueprintWrapper;
/**
 * 
 */
UCLASS()
class UE_PROJECT_CS193N_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:

	ASGameModeBase();
	virtual void StartPlay() override;
	void OnActorKilled(AActor* VictimActor, AActor* Killer);
protected:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> SpawnBotClass;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* diffcultCurve;

	FTimerHandle TimerHandle_SpawnBots;
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimeInterval;
	UFUNCTION()
	void SpawnBotTimeElapsed();
	UFUNCTION()
	void OnQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	//void OnActorKilled(AActor* VictimActor, AActor* Killer);
	UFUNCTION()
	void RespawnPlayerElapsed(AController* controller);

	UFUNCTION(Exec)
	void KillAll();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "SGameModeBase.generated.h"



class UEnvQuery;
class UCurveFloat;
class UEnvQueryInstanceBlueprintWrapper;
class ASItemBase;
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
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	UEnvQuery* ItemQuery;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TSubclassOf<ASItemBase> HealthPotionClass;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TSubclassOf<ASItemBase> CreditsCoinClass;






	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> SpawnBotClass;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* diffcultCurve;

	FTimerHandle TimerHandle_SpawnItem;

	FTimerHandle TimerHandle_SpawnBots;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimeInterval;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	float SpawnItem_TimeInterval;

	UFUNCTION()
	void SpawnBotTimeElapsed();

	UFUNCTION()
	void SpawnItemTimeElapsed();

	UFUNCTION()
	void OnQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void OnItemQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	//void OnActorKilled(AActor* VictimActor, AActor* Killer);
	UFUNCTION()
	void RespawnPlayerElapsed(AController* controller);

	UFUNCTION(Exec)
	void KillAll();
};

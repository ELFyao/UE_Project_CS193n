// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "AI/SAICharacter.h"
#include "EngineUtils.h"
#include "SAttributeComponent.h"
#include "../SCharacter.h"
#include "SCVarObject.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), false, TEXT("Enable Spawning of bots via timer"), ECVF_Cheat);

ASGameModeBase::ASGameModeBase()
{
	SpawnTimeInterval = 2.0f;
}

void ASGameModeBase::KillAll()
{
	for (TActorIterator<ASAICharacter>It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;
		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot);
		//if (ensure(AttributeComp) && USAttributeComponent::IsActorAlive(Bot))        //需要实现AttributeComp
		if (USAttributeComponent::IsActorAlive(Bot))
		{
			AttributeComp->Kill(this);
		}
	}
}


void ASGameModeBase::StartPlay()
{
	Super::StartPlay();
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimeElapsed, SpawnTimeInterval, true);
}

void ASGameModeBase::SpawnBotTimeElapsed()
{
	if (CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot spwaning disabled via cvar 'CVarSpawnBots'."));
		return;
	}
	
	int NumofAliveBots = 0;
	for (TActorIterator<ASAICharacter>It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;
		//USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot);
		//if (ensure(AttributeComp) && USAttributeComponent::IsActorAlive(Bot))        //需要实现AttributeComp
		if (USAttributeComponent::IsActorAlive(Bot))
		{
			NumofAliveBots++;
		}
	}
	UE_LOG(LogTemp, Log, TEXT("Found %i Alive Robot"), NumofAliveBots);
	float MaxbotCount = 10.0f;
	if (diffcultCurve)
	{
		MaxbotCount = diffcultCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	if (NumofAliveBots >= MaxbotCount) {
		UE_LOG(LogTemp, Log, TEXT("At Maximum bot Capacity, Skipping bot Spawn."));
		return;
	}
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryFinished);
	}
	
}

void ASGameModeBase::OnQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
 	 if (QueryStatus != EEnvQueryStatus::Success) {
		 UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed"));
  		return;
  	 }
	 
	

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if (Locations.IsValidIndex(0)) {
 	//	FVector GenerateLocation = Locations[0];
 	//	FVector height = FVector(0, 0, 180);
 	//	GenerateLocation = GenerateLocation + height;
		//GetWorld()->SpawnActor<AActor>(SpawnBotClass, GenerateLocation, FRotator::ZeroRotator);
		GetWorld()->SpawnActor<AActor>(SpawnBotClass, Locations[0], FRotator::ZeroRotator);
		if (CVarDrawDebugShape.GetValueOnGameThread()){
			DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 20, FColor::Blue, false, 60.0f);
		}
	}
		

}

void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	ASCharacter* Player = Cast<ASCharacter>(VictimActor);
	if (Player) {
		FTimerHandle TimerHandle_RespawnDelay;
		
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());

		float RespwanDelay = 5.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespwanDelay, false);

	}
	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));
}

void ASGameModeBase::RespawnPlayerElapsed(AController* controller)
{
	if (ensure(controller))
	{
		controller->UnPossess();
		RestartPlayer(controller);
	}
}


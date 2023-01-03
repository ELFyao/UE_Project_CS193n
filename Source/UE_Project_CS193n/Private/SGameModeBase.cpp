// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "AI/SAICharacter.h"
#include "EngineUtils.h"
#include "SAttributeComponent.h"
#include "SItemBase.h"
#include "../SCharacter.h"
#include "SCVarObject.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), false, TEXT("Enable Spawning of bots via timer"), ECVF_Cheat);
static TAutoConsoleVariable<bool> CVarSpawnItems(TEXT("su.SpawnItems"), false, TEXT("Enable Spawning Item"), ECVF_Cheat);

ASGameModeBase::ASGameModeBase()
{
	SpawnTimeInterval = 2.0f;

	SpawnItem_TimeInterval = 5.0f;
}

void ASGameModeBase::KillAll()
{
	for (TActorIterator<ASAICharacter>It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;
		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot);
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

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnItem, this, &ASGameModeBase::SpawnItemTimeElapsed, SpawnItem_TimeInterval, false);
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

void ASGameModeBase::SpawnItemTimeElapsed()
{
	if (CVarSpawnItems.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot spwaning disabled via cvar 'CVarSpawnItems'."));
		return;
	}
	int NumofItems = 0;

	for (TActorIterator<ASItemBase>It(GetWorld()); It; ++It)
	{
		NumofItems++;
	}
	UE_LOG(LogTemp, Log, TEXT("Found %i items"), NumofItems);

	float MaxItemCount = 6.0f;

	if (NumofItems >= MaxItemCount) {
		UE_LOG(LogTemp, Log, TEXT("At Maximum item Capacity, Skipping item Spawn."));
		return;
	}

	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, ItemQuery, this, EEnvQueryRunMode::AllMatching, nullptr);
	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnItemQueryFinished);
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
		GetWorld()->SpawnActor<AActor>(SpawnBotClass, Locations[0], FRotator::ZeroRotator);
		if (CVarDrawDebugShape.GetValueOnGameThread()) {
			DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 20, FColor::Blue, false, 60.0f);
		}
	}
}

void ASGameModeBase::OnItemQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success) {
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed"));
		return;
	}
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	float MaxICoinCount = 4.0f;

	float MaxIPotionCount = 2.0f;

	float MaxItemCount = MaxIPotionCount + MaxICoinCount;

	TArray<FVector> pickedItemLocations;

	while ((pickedItemLocations.Num() < MaxICoinCount) && Locations.Num() > 0)
	{
		FVector pickedLocation;

		int index = FMath::RandRange(0, Locations.Num() - 1);

		pickedLocation = Locations[index];
		Locations.RemoveAt(index);

		bool isvalid = true;

		for (FVector location : pickedItemLocations) {
			if (FVector::Distance(pickedLocation, location) < 800.f) {
				isvalid = false;
			}
		}
		if (isvalid == false)
		{
			continue;
		}
		pickedItemLocations.Add(pickedLocation);
	}

	float spawnedPotionCount = 0;
	float spawnedCoinCount = 0;

	for (int i = 0; i < pickedItemLocations.Num(); i++)
	{
		if (i == 0)
		{
			GetWorld()->SpawnActor<AActor>(HealthPotionClass, pickedItemLocations[i], FRotator::ZeroRotator);
			spawnedPotionCount++;
			continue;
		}
		if (i == 1)
		{
			GetWorld()->SpawnActor<AActor>(CreditsCoinClass, pickedItemLocations[i], FRotator::ZeroRotator);
			spawnedCoinCount++;
			continue;
		}
		bool coin = FMath::RandBool();
		
		if (coin && spawnedPotionCount < MaxIPotionCount)
		{
			GetWorld()->SpawnActor<AActor>(HealthPotionClass, pickedItemLocations[i], FRotator::ZeroRotator);
			spawnedPotionCount++;
		}
		else
		{
			GetWorld()->SpawnActor<AActor>(CreditsCoinClass, pickedItemLocations[i], FRotator::ZeroRotator);
			spawnedCoinCount++;
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


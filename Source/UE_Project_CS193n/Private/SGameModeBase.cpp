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
#include "../Public/ASPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "../Public/SSaveGame.h"
#include "GameFramework/GameStateBase.h"
#include "../Public/SGameplayInterface.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enable Spawning of bots via timer"), ECVF_Cheat);
static TAutoConsoleVariable<bool> CVarSpawnItems(TEXT("su.SpawnItems"), false, TEXT("Enable Spawning Item"), ECVF_Cheat);

ASGameModeBase::ASGameModeBase()
{
	SpawnTimeInterval = 2.0f;

	CreditsPerKill = 100;

	SpawnItem_TimeInterval = 5.0f;

	SlotName = "SaveGame01";
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
	LoadWorldEnviron();
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
	
	//@fixme remove hard coding


	float MaxbotCount = 0.0f;    

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

	//@fixme remove hard coding
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

	//½±Àø

	APawn* Killerpawn = Cast<APawn>(Killer);
	if (Killerpawn)
	{
		if (AASPlayerState* playerStates = Killerpawn->GetPlayerState<AASPlayerState>())
		{
			playerStates->AddCredits(CreditsPerKill);
		}
	}

}

void ASGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	AASPlayerState* PS = NewPlayer->GetPlayerState<AASPlayerState>();
	if (PS)
	{
		PS->LoadPlayerState(CurrentSaveGame);
	}

}

void ASGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	LoadSaveGame();
}

void ASGameModeBase::RespawnPlayerElapsed(AController* controller)
{
	if (ensure(controller))
	{
		controller->UnPossess();
		RestartPlayer(controller);
	}
}

void ASGameModeBase::WriteSaveGame()
{
	for (int32 i = 0; i < GameState->PlayerArray.Num(); i++) {
		AASPlayerState* PS = Cast<AASPlayerState>(GameState->PlayerArray[i]);
		if (PS)
		{
			PS->SavePlayerState(CurrentSaveGame);
			break;
		}
	}

	CurrentSaveGame->SavedActors.Empty();

	for (FActorIterator It(GetWorld()); It; ++It) {
		AActor* Actor = *It;
		if(!Actor->Implements<USGameplayInterface>()){
			continue;
		}
		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetName();
		ActorData.transform = Actor->GetActorTransform();

		FMemoryWriter MemWriter(ActorData.ByteData);

		// pass the array to fill with data from Actor
		FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);

		//find only variables with property(SaveGame)
		Ar.ArIsSaveGame = true;
		Actor->Serialize(Ar);




		CurrentSaveGame->SavedActors.Add(ActorData);
	 }



	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);

}



void ASGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (CurrentSaveGame == nullptr) {
			UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame Data."));
			return;
		}
		UE_LOG(LogTemp, Log, TEXT("Loaded SaveGame Data."));
	}
	else
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::CreateSaveGameObject(USSaveGame::StaticClass()));
		UE_LOG(LogTemp, Log, TEXT("Create New SaveGame Data."));
	}
}





void ASGameModeBase::LoadWorldEnviron()
{
	if (CurrentSaveGame!= nullptr)
	{
		//CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		//if (CurrentSaveGame == nullptr) {
		//	UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame Data."));
		//	return;
		//}
		//UE_LOG(LogTemp, Log, TEXT("Loaded SaveGame Data."));
		for (FActorIterator It(GetWorld()); It; ++It) {
			AActor* Actor = *It;
			if (!Actor->Implements<USGameplayInterface>()) {
				continue;
			}

			for (FActorSaveData ActorData : CurrentSaveGame->SavedActors) {

				if (ActorData.ActorName == Actor->GetName())
				{
					Actor->SetActorTransform(ActorData.transform);

					FMemoryReader MemReader(ActorData.ByteData);
					FObjectAndNameAsStringProxyArchive Ar(MemReader, true);

					//find only variables with property(SaveGame)
					Ar.ArIsSaveGame = true;
					Actor->Serialize(Ar);

					ISGameplayInterface::Execute_OnActorLoaded(Actor);

					break;
				}
			}
		}



	}
	else
	{
		LoadSaveGame();
	}
}

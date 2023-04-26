// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SSaveGame.generated.h"

USTRUCT()
struct FActorSaveData 
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FString ActorName;


	UPROPERTY()
	FTransform transform;

	UPROPERTY()
	TArray<uint8>ByteData;
};
/**
 * 
 */
UCLASS()
class UE_PROJECT_CS193N_API USSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY()
	int32 Credits;

	UPROPERTY()
	TArray<FActorSaveData> SavedActors;

};

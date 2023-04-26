 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "SAction.generated.h"


class USActionComponent;

USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()
public:
	UPROPERTY()
	bool bIsRunning;
	UPROPERTY()
	AActor* InstigatorActor;
};
/**
 * 
 */
UCLASS(Blueprintable)
class UE_PROJECT_CS193N_API USAction : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
		UTexture2D* Icon; 

	UPROPERTY(Replicated)
	USActionComponent* ActionComp;

	UPROPERTY(ReplicatedUsing = "OnRep_RepData")
	FActionRepData RepData;
		/*bool bIsRunning;*/

	UFUNCTION()
	void OnRep_RepData();

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags; 

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	UFUNCTION(BlueprintCallable, Category = "Action")
	USActionComponent* GetOwingComponent() const;

	float TimeStarted;

public:
	void Initialize(USActionComponent* NewActionComp);

	UPROPERTY(EditDefaultsOnly, Category = "Action")
    bool bAutoStart;

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool GetIsRunning();

	float GetTimeRemaining() const;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* InstigatorActor);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* InstigatorActor);

	UPROPERTY(EditDefaultsOnly, Category="Action")
	FName ActionName;

	UWorld* GetWorld() const override;


	bool IsSupportedForNetworking() const override {
		return true;
	}
};

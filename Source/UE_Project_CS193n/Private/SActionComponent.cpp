// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionComponent.h"

// Sets default values for this component's properties
USActionComponent::USActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void USActionComponent::AddAction(TSubclassOf<USAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}
	USAction* NewAction = NewObject<USAction>(this, ActionClass);
	if (ensure(NewAction))
	{
		Actions.Add(NewAction);
	}
}

bool USActionComponent::StartActionByName(AActor* InstigatorActor, FName ActionName)
{
	for (USAction* ActionClass : Actions) {
		if (ActionClass && ActionClass->ActionName == ActionName && ActionClass->CanStart(InstigatorActor))
		{
			FString DebugMsg = ("Action Start Failed :" + ActionName.ToString());
			GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, DebugMsg);

			ActionClass->StartAction(InstigatorActor);
			return true;
		}
	}

	return false;
}

bool USActionComponent::StopActionByName(AActor* InstigatorActor, FName ActionName)
{
	for (USAction* ActionClass : Actions) {
		if (ActionClass && ActionClass->ActionName == ActionName)
		{
			if (ActionClass->GetIsRunning())
			{
				ActionClass->StopAction(InstigatorActor);
				return true;
			}
		}
	}
	return false;
}

// Called when the game starts
void USActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FString DebugMsg = GetNameSafe(GetOwner()) + ":" + AcitveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);

	// ...
}



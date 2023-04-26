// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionComponent.h"
#include "../UE_Project_CS193n.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"

// Sets default values for this component's properties
USActionComponent::USActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

	// ...
}


void USActionComponent::AddAction(AActor* InstigatorActor,TSubclassOf<USAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}


	if (!GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Client attempting to AddAction.[Class: %s]"), *GetNameSafe(ActionClass));
		return;
	}



	//@FIXME: BUG HAS FIXED IN UE5
	USAction* NewAction = NewObject<USAction>(GetOwner(), ActionClass);
	if (ensure(NewAction))
	{
		NewAction->Initialize(this);
		Actions.Add(NewAction);
		if (NewAction->bAutoStart &&ensure(NewAction->CanStart(InstigatorActor)))
		{
			NewAction->StartAction(InstigatorActor);
		}
	}
}

void USActionComponent::RemoveAction(USAction* ActionToRemove)
{
	if (ensure(ActionToRemove && !ActionToRemove->GetIsRunning()))
	{
		Actions.Remove(ActionToRemove);
	}
	
}


bool USActionComponent::StartActionByName(AActor* InstigatorActor, FName ActionName)
{
	for (USAction* ActionClass : Actions) {
		if (ActionClass && ActionClass->ActionName == ActionName && ActionClass->CanStart(InstigatorActor))
		{
			FString DebugMsg = ("Action Start Failed :" + ActionName.ToString());
			GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, DebugMsg);


			if (!GetOwner()->HasAuthority())
			{
				ServerStartAction(InstigatorActor, ActionName);
			}
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
				if (!GetOwner()->HasAuthority())
				{
					ServerStopAction(InstigatorActor, ActionName);
				}
				ActionClass->StopAction(InstigatorActor);
				return true;
			}
		}
	}
	return false;
}

void USActionComponent::ServerStartAction_Implementation(AActor* InstigatorActor, FName ActionName)
{
	StartActionByName(InstigatorActor, ActionName);
}

void USActionComponent::ServerStopAction_Implementation(AActor* InstigatorActor, FName ActionName)
{
	StopActionByName(InstigatorActor, ActionName);
}

// Called when the game starts
void USActionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority())
	{
		for (TSubclassOf<USAction> ActionClass : ActionClasses)
		{
			AddAction(GetOwner(), ActionClass);
		}
	}


	// ...
	
}


// Called every frame
void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//FString DebugMsg = GetNameSafe(GetOwner()) + ":" + AcitveGameplayTags.ToStringSimple();
	//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);
	for (USAction* Action : Actions)
	{
		FColor TextColor = Action->GetIsRunning() ? FColor::Blue : FColor::White;
		FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s"),
			*GetNameSafe(GetOwner()),
			*GetNameSafe(Action));
		LogScreen(this, ActionMsg, TextColor, 0.0f);
	}

	// ...
}

bool USActionComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomthing = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (USAction* Action : Actions)
	{
		if (Action) {
			WroteSomthing |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}
	return WroteSomthing;
}



void USActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USActionComponent, Actions);
}


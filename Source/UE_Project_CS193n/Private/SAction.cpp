// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"
#include "SActionComponent.h"
#include "../UE_Project_CS193n.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Actor.h"





bool USAction::CanStart_Implementation(AActor* InstigatorActor)
{
	USActionComponent *OwningComp = GetOwingComponent();
	if (OwningComp)
	{
		if (OwningComp->AcitveGameplayTags.HasAny(BlockedTags))
		{
			return false;
		}
	}
	return true;
}


void USAction::Initialize(USActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
}

bool USAction::GetIsRunning()
{
	return RepData.bIsRunning;
}

float USAction::GetTimeRemaining() const
{

}

void USAction::StartAction_Implementation(AActor* InstigatorActor)
{
	UE_LOG(LogTemp, Log, TEXT("Runing: %s"), *GetNameSafe(this));
	//LogScreen(this, FString::Printf(TEXT("Started : %s"), *ActionName.ToString()), FColor::Green);
	USActionComponent* OwingComp = GetOwingComponent();
	if (OwingComp)
	{
		OwingComp->AcitveGameplayTags.AppendTags(GrantsTags);
	}

	RepData.bIsRunning = true;
	RepData.InstigatorActor = InstigatorActor;
	
	TimeStarted = GetWorld()->TimeSeconds;

	GetOwingComponent()->OnActionStarted.Broadcast(GetOwingComponent(), this);
}

void USAction::StopAction_Implementation(AActor* InstigatorActor)
{
	UE_LOG(LogTemp, Log, TEXT("Runing: %s"), *GetNameSafe(this));
	//LogScreen(this, FString::Printf(TEXT("Stopped : %s"), *ActionName.ToString()), FColor::Red);
	USActionComponent* OwingComp = GetOwingComponent();

	//ensureAlways(bIsRunning);

	if (OwingComp)
	{
		OwingComp->AcitveGameplayTags.RemoveTags(GrantsTags);
	}

	RepData.bIsRunning = false;
	RepData.InstigatorActor = InstigatorActor;
	GetOwingComponent()->OnActionStopped.Broadcast(GetOwingComponent(), this);
}


UWorld* USAction::GetWorld() const
{

	//@FIXME: IT SEEMS THE BUG HAS FIXED IN UE5
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor) {
		return Actor->GetWorld();
	}
	return NULL;
}


void USAction::OnRep_RepData()
{
	if (RepData.bIsRunning) {
		StartAction(RepData.InstigatorActor);
	}
	else
	{
		StopAction(RepData.InstigatorActor);
	}
}

USActionComponent* USAction::GetOwingComponent() const
{
	//AActor* Actor = Cast<AActor>(GetOuter());
	//
	//return Actor->GetComponentByClass(USActionComponent::StaticClass());
	// 
	// 
	return ActionComp;
	//return Cast<USActionComponent>(GetOuter());
}

void USAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAction, RepData);
	DOREPLIFETIME(USAction, ActionComp);
}
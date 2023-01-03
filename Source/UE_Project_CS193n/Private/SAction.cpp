// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"
#include "SActionComponent.h"





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


bool USAction::GetIsRunning()
{
	return bIsRunning;
}



void USAction::StartAction_Implementation(AActor* InstigatorActor)
{
	UE_LOG(LogTemp, Log, TEXT("Runing: %s"), *GetNameSafe(this));
	USActionComponent* OwingComp = GetOwingComponent();
	if (OwingComp)
	{
		OwingComp->AcitveGameplayTags.AppendTags(GrantsTags);
	}

	bIsRunning = true;
}

void USAction::StopAction_Implementation(AActor* InstigatorActor)
{
	UE_LOG(LogTemp, Log, TEXT("Runing: %s"), *GetNameSafe(this));
	USActionComponent* OwingComp = GetOwingComponent();

	ensureAlways(bIsRunning);

	if (OwingComp)
	{
		OwingComp->AcitveGameplayTags.RemoveTags(GrantsTags);
	}

	bIsRunning = false;
}


UWorld* USAction::GetWorld() const
{
	UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
	if (Comp) {
		return Comp->GetWorld();
	}
	return NULL;
}


USActionComponent* USAction::GetOwingComponent() const
{
	return Cast<USActionComponent>(GetOuter());
}
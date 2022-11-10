// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"
#include "SGameModeBase.h"

// Sets default values for this component's properties
static TAutoConsoleVariable<float> CVarHealthMultiplier(TEXT("su.HealthMultiplier"), 1.0f, TEXT("Multiplier Health damage not influnence healing"), ECVF_Cheat);

USAttributeComponent::USAttributeComponent()
{
	//Health = 100;
	HealthMax = 100;
	Health = HealthMax;
	LowHealthThreshold = 40;
	// ...
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor) {
		return Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	}
	return NULL;
	}




bool USAttributeComponent::IsActorAlive(AActor* FromActor)
{
	USAttributeComponent* AttributeComp = GetAttributes(FromActor);
	if (AttributeComp)
	{
		return AttributeComp->GetIsAlive();
	}
	return false;
}

bool USAttributeComponent::GetIsAlive()
{
	return Health > 0.0f;
}



bool USAttributeComponent::GetIsLowHealth()
{
	return Health <= LowHealthThreshold;
}



float USAttributeComponent::GetHealth()
{
	return Health;
}



float USAttributeComponent::GetMaxHitPoint()
{
	return HealthMax;
}

bool USAttributeComponent::GetIsFull()
{
	return Health >= HealthMax;
}

void USAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool USAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetMaxHitPoint());
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged()) {
		return false;
	}

	if (Delta < 0.0f) {
		Delta = Delta * CVarHealthMultiplier.GetValueOnGameThread();
	}


	float old_Health = Health;
	Health += Delta;
	Health = FMath::Clamp<float>(Health, 0, HealthMax);
	float actualDelta = Health - old_Health;
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, actualDelta);

	if (actualDelta < 0.0f && Health == 0.0f) {
		ASGameModeBase* GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();
		if (GM) {
			GM->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}

	return actualDelta != 0;
}

// Called every frame



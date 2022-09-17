// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	//Health = 100;
	HealthMax = 100;
	Health = HealthMax;
	// ...
}

bool USAttributeComponent::GetIsAlive()
{
	return Health > 0.0f;
}



bool USAttributeComponent::GetIsFull()
{
	return Health >= HealthMax;
}

void USAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;
	Health = FMath::Clamp<float>(Health, 0, HealthMax);
	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
	return true;
}

// Called every frame



// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemBase.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/Actor.h"

// Sets default values
ASItemBase::ASItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	
	bIsCooldown = true;
	CooldownSeconds = 5.0f;
}

void ASItemBase::CooldownOver()
{
	BaseMesh->SetVisibility(true);
	bIsCooldown = true;
}

bool ASItemBase::PlayFunction(APawn* InstigatorPawn)
{
	return true;
}

void ASItemBase::Interact_Implementation(APawn* InstigatorPawn)
{
	if (bIsCooldown && PlayFunction(InstigatorPawn))
	{
		BaseMesh->SetVisibility(false);
		bIsCooldown = false;
		GetWorldTimerManager().SetTimer(CooldownTimer, this, &ASItemBase::CooldownOver, CooldownSeconds);
	}

}


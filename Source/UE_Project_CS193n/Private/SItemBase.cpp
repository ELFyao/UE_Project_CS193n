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
	
}

// Called when the game starts or when spawned
void ASItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// void ASItemBase::Pick_Implementation()               //BlueprintNativeEvent 必须在后面加上 _Implementation.
// {
// 	BaseMesh->SetVisibility(false);
// 	FTimerDelegate RespawnDelegate = FTimerDelegate::CreateUObject(GetWorld(), &BaseMesh->SetVisibility, true);
// 	GetWorldTimerManager().SetTimer(CooldownTimer, RespawnDelegate, 5.0f);
// 
// }

void ASItemBase::Pick_Implementation()
{

}

// Called every frame
void ASItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASItemBase::Interact_Implementation(APawn* InstigatorPawn)
{

}


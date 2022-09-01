// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectileDash_2.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

ASProjectileDash_2::ASProjectileDash_2()
{
	PrimaryActorTick.bCanEverTick = true;
	TeleportDelay = 0.2f;
	DetonateDelay = 0.2f;
	MovementComp->InitialSpeed = 6000.f;
}


void ASProjectileDash_2::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(TimerHandle_DelayDetonate, this, &ASProjectileDash_2::Explode, DetonateDelay);
}

void ASProjectileDash_2::Explode_Implementation()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_DelayDetonate);

	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());

	EffectComp->DeactivateSystem();

	MovementComp->StopMovementImmediately();

	SetActorEnableCollision(false);

	FTimerHandle TimerHandle_DelayTeleport;
	GetWorldTimerManager().SetTimer(TimerHandle_DelayTeleport, this, &ASProjectileDash_2::TeleportInstigator, TeleportDelay);
}

void ASProjectileDash_2::TeleportInstigator()
{
	AActor* ActorToTeleport = GetInstigator();
	if (ensure(ActorToTeleport)) {
		ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation(), false, false);
	}
}

void ASProjectileDash_2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//void ASProjectileDash_2::ProjectileEffect()
//{
//
//}

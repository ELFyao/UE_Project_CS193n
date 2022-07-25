// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectileParent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASProjectileParent::ASProjectileParent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AttackSpeed = 1000.f;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");

	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");

	EffectComp->SetupAttachment(SphereComp);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");

	MovementComp->InitialSpeed = AttackSpeed;

	MovementComp->bRotationFollowsVelocity = true;

	MovementComp->bInitialVelocityInLocalSpace = true;


	SphereComp->SetCollisionProfileName("Projectile");



}

// Called when the game starts or when spawned
void ASProjectileParent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASProjectileParent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASProjectileParent::ProjectileAttack_Implementation(APawn* InstigatorPawn)
{
	
}


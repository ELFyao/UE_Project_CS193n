// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectileParent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASProjectileParent::ASProjectileParent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AttackSpeed = 1000.f;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");

	SphereComp->SetCollisionProfileName("Projectile");
	
	SphereComp->OnComponentHit.AddDynamic(this, &ASProjectileParent::OnActorHit);        //这里提前注册了碰撞函数。

	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");

	EffectComp->SetupAttachment(SphereComp);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");

	MovementComp->InitialSpeed = AttackSpeed;

	MovementComp->bRotationFollowsVelocity = true;

	MovementComp->bInitialVelocityInLocalSpace = true;

	MovementComp->ProjectileGravityScale = 0.0f;
}

// Called when the game starts or when spawned


void ASProjectileParent::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}


void ASProjectileParent::Explode_Implementation()
{
	if (ensure(!IsPendingKill())) {      //修改处
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());
		
		Destroy();
	}
}



void ASProjectileParent::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
}

// Called every frame
void ASProjectileParent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}




// Fill out your copyright notice in the Description page of Project Settings.


#include "SDashProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet//GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values



ASDashProjectile::ASDashProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AttackSpeed = 2000.f;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");

	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");


	EffectComp->SetupAttachment(SphereComp);

	EmitterComp = CreateDefaultSubobject<UParticleSystem>("EmitterComp");

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");

	MovementComp->InitialSpeed = AttackSpeed;

	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
	SphereComp->SetCollisionProfileName("Projectile"); 
	DashLocation = SphereComp->GetComponentLocation();

	ConstructorHelpers::FObjectFinder<UParticleSystem> EmitterCompTemp(TEXT("/Game/ParagonGideon/FX/Particles/Gideon/Abilities/Primary/FX/P_Gideon_Primary_HitWorld"));
	if (EmitterCompTemp.Succeeded())
	{
		EmitterComp = EmitterCompTemp.Object;
	}
	
}

// Called when the game starts or when spawned
void ASDashProjectile::BeginPlay()
{
	Super::BeginPlay();
	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryDash, this, &ASDashProjectile::PrimaryDash, 0.2f);
}

void ASDashProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComp->OnComponentHit.AddDynamic(this,  &ASDashProjectile::OnHit);
}

void ASDashProjectile::PrimaryDash()
{
	DashLocation = SphereComp->GetComponentLocation();
	FRotator DashDirection = GetActorRotation();//GetOwner()->GetActorRotation();

	
	
	FTransform SpawnTM;
	SpawnTM = FTransform(DashDirection, DashLocation);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EmitterComp, DashLocation,DashDirection, true, EPSCPoolMethod::None, true);
	MovementComp->Deactivate();
	APawn* InstigatorPawn = GetInstigator();
	FTimerDelegate RespawnDelegate = FTimerDelegate::CreateUObject(this, &ASDashProjectile::ProjectileAttack_Implementation, InstigatorPawn);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryDash, RespawnDelegate, 0.2f, false);
	
}



void ASDashProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor) {
		GetWorldTimerManager().ClearTimer(TimerHandle_PrimaryDash);
		DashLocation += FVector::UnitZ() * 50;
		PrimaryDash();
	}
	
}

// Called every frame
void ASDashProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASDashProjectile::ProjectileAttack_Implementation(APawn* InstigatorPawn)
{
	InstigatorPawn->SetActorLocation(DashLocation);
	Destroy();
}


//void ASDashProjectile::ProjectileAttack_Implementation(APawn* InstigatorPawn)
//{
//	
//
//}


// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "SAttributeComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SGameplayFunctionLibrary.h"


// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AttackSpeed = 1000.f;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	
	RootComponent = SphereComp;
	
	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");

	DamageAmount = -20.0f;

	EffectComp->SetupAttachment(SphereComp);
	
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");

	MovementComp->InitialSpeed = AttackSpeed;

	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;


	AudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComp");

	AudioComp->SetupAttachment(RootComponent);

	//ImpactAudioComp = CreateDefaultSubobject<UAudioComponent>("ImpactAudioComp");

	//设置球体碰撞的规则。
	//SphereComp->SetCollisionObjectType(ECC_WorldDynamic);
	//SphereComp->SetCollisionResponseToAllChannels(ECR_Overlap);
	//SphereComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	SphereComp->SetCollisionProfileName("Projectile");
}

// Called when the game starts or when spawned
void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	//ImpactAudioComp->Deactivate();
}

void ASMagicProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());
	AudioComp->Deactivate();
	UGameplayStatics::PlaySoundAtLocation(this, ImpactAudio, GetActorLocation(), GetActorRotation(),1.0,1.0,0.0);
	//UGameplayStatics::PlayWorldCameraShake(this, ImpactCameraShake, GetActorLocation(), 50, 150);
	/*ImpactAudioComp->Activate();*/
	Destroy();
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator()) {
		//UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());
		//USAttributeComponent *AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		//if (AttributeComp) {
		//	AttributeComp->ApplyHealthChange(GetInstigator(),DamageAmount);
		//	UGameplayStatics::PlayWorldCameraShake(this, ImpactCameraShake, GetActorLocation(), 100, 500);
		//	Destroy();
		//}
		if (USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, DamageAmount, SweepResult))
		{
			UGameplayStatics::PlayWorldCameraShake(this, ImpactCameraShake, GetActorLocation(), 100, 500);
			Destroy();
		}

	}
}

void ASMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);
	SphereComp->OnComponentHit.AddDynamic(this, &ASMagicProjectile::OnActorHit);
}

// Called every frame
void ASMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


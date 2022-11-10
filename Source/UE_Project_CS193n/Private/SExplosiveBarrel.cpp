// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"

#include "Components/StaticMeshComponent.h"

#include "PhysicsEngine/RadialForceComponent.h"
#include "SAttributeComponent.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
	StaticMeshComp->SetSimulatePhysics(true);
	//StaticMeshComp->SetNotifyRigidBodyCollision(true);
	StaticMeshComp->SetCollisionProfileName("PhysicsActor");
	//StaticMeshComp->SetCollisionObjectType(ECC_PhysicsBody);
	//StaticMeshComp->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnCompHit);



	RootComponent = StaticMeshComp;

	RadialForceComp->SetupAttachment(RootComponent);

	RadialForceComp->Radius = 1000.f;
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->bAutoActivate = false;
	//RadialForceComp->bIgnoreOwningActor = true; 

	//RadialForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);



	//StaticMeshComp->OnComponentHit.Add(&ASExplosiveBarrel::OnCompHit);




}

// Called when the game starts or when spawned
void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
}

void ASExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	StaticMeshComp->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnCompHit);
}

void ASExplosiveBarrel::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	RadialForceComp->FireImpulse();
	if (OtherActor)
	{
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttributeComp) {
			AttributeComp->ApplyHealthChange(this, -50);
		}
	}
	//if ((RadialForceComp != NULL)&&(OtherActor!=NULL)&&(OtherActor != this) && OtherComp!= NULL) {

	//}
	UE_LOG(LogTemp, Log, TEXT("OnActorHit in Explosive Barrel"));
	UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s, at game time %f"), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);
	FString CombineString = FString::Printf(TEXT("Hit at location: %s"), *Hit.ImpactPoint.ToString());
	DrawDebugString(GetWorld(), Hit.ImpactPoint, CombineString, nullptr, FColor::Green, 2.0f, true);
}

// Called every frame
void ASExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


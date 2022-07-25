// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttackComponent.h"
#include "SProjectileInterface.h"

void USAttackComponent::ProjectileDash()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = Cast<APawn>(GetOwner());

}

// Sets default values for this component's properties
USAttackComponent::USAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}






void USAttackComponent::SpawnDashProjectile(FTransform SpawnTM, FActorSpawnParameters SpawnParams)
{
	SpawnProjectile(DashProjectileClass, SpawnTM, SpawnParams);
}



void USAttackComponent::SpawnProjectile(TSubclassOf<AActor> Projectile, FTransform SpawnTM, FActorSpawnParameters SpawnParams)
{
	if (ensure(Projectile)) {
		if (Projectile->Implements<USProjectileInterface>())
		{

		}
		GetWorld()->SpawnActor<AActor>(Projectile, SpawnTM, SpawnParams);
	}
}

// Called when the game starts
void USAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


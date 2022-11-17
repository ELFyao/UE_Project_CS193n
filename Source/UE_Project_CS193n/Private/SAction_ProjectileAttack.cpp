// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_ProjectileAttack.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"


USAction_ProjectileAttack::USAction_ProjectileAttack()
{
	AttackDistance = 5000.f;
	HandSocketName = "Muzzle_01";
	TimeToHitParams = "TimeToHit";
	AttackAnimDelay = 0.2f;
}



UWorld* USAction_ProjectileAttack::GetWorld() const
{
	UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
	if (Comp)
	{
		return Comp->GetWorld();
	}
	return nullptr;
}

void USAction_ProjectileAttack::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);

	ACharacter* InstigatorCharater = Cast<ACharacter>(InstigatorActor);
	if (InstigatorCharater)
	{
		InstigatorCharater->PlayAnimMontage(AttackAnim);
		UGameplayStatics::SpawnEmitterAttached(HandFlashVFX, InstigatorCharater->GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
		
		FTimerHandle TimerHandle_AttackDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "AttackDelayElapsed", InstigatorCharater);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay, false);
	}
}


void USAction_ProjectileAttack::AttackDelayElapsed(ACharacter* InstigatorActor)
{
	if (ensure(ProjectileClass))
	{
		FVector handsLocation = InstigatorActor->GetMesh()->GetSocketLocation(HandSocketName);

		/*
		* assignment_2 part3 start here.
		*/

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorActor;



		// line trace
		FVector ControlLocation = InstigatorActor->GetPawnViewLocation();
		FRotator ControlRotation = InstigatorActor->GetControlRotation();

		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);      //这里由ECC_PhysicsBody 改成了ECC_Pawn
		FVector End = ControlLocation + (ControlRotation.Vector() * AttackDistance);

		FHitResult Hit;

		FVector AttackEnd;

		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(InstigatorActor);
		//bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, ControlLocation, End, ObjectQueryParams);
		bool bBlockingHit = GetWorld()->SweepSingleByObjectType(Hit, ControlLocation, End, FQuat::Identity, ObjectQueryParams, Shape, Params);
		if (bBlockingHit) {
			AttackEnd = Hit.ImpactPoint;
		}
		else
		{
			AttackEnd = End;
		}

		FRotator AttackDirection = (AttackEnd - handsLocation).Rotation();
		FTransform SpawnTM = FTransform(AttackDirection, handsLocation);
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	}
	StopAction(InstigatorActor);
}



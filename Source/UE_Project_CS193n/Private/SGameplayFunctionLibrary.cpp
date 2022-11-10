// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameplayFunctionLibrary.h"
#include "SAttributeComponent.h"

bool USGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(TargetActor);
	if (AttributeComp)
	{
		return AttributeComp->ApplyHealthChange(DamageCauser, DamageAmount);
	}
	return false;
}

bool USGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult &HitResult)
{
	if (ApplyDamage(DamageCauser,TargetActor,DamageAmount))
	{
		UPrimitiveComponent* HitComp = HitResult.GetComponent();
		if (HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
		{
			FVector ShootDirection = (HitResult.TraceStart - HitResult.TraceEnd);
			ShootDirection.Normalize(1.0);
			FVector NormalVector = (HitResult.ImpactNormal);
			NormalVector.Normalize(1.0);
			FVector ImpactDirection = NormalVector + ShootDirection;
			HitComp->AddImpulseAtLocation(-ImpactDirection * 300000.f, HitResult.ImpactPoint, HitResult.BoneName);
		}
		return true;
	}
	return false;
}
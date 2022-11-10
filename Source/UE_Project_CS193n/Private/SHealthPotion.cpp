// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"
#include "SAttributeComponent.h"

ASHealthPotion::ASHealthPotion()
{
	bIsCooldown = true;
}

void ASHealthPotion::CooldownOver()
{
	BaseMesh->SetScalarParameterValueOnMaterials("bIsUsed", 1);
	bIsCooldown = true;
}

void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	//apply functionality of Health Potion
	//USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	if (InstigatorPawn) {
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttributeComp) {
			if (AttributeComp->GetIsFull() == false && bIsCooldown)
			{
				AttributeComp->ApplyHealthChange(InstigatorPawn, HealVolume);
				bIsCooldown = false;
				Pick();
			}
		}
	}
}

 void ASHealthPotion::Pick_Implementation()
 {
	// apply vision effect of Health Potion after healing;
	 BaseMesh->SetScalarParameterValueOnMaterials("bIsUsed", -1);
	 GetWorldTimerManager().SetTimer(CooldownTimer, this, &ASHealthPotion::CooldownOver, 5.0f);
 }

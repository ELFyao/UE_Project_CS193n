// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckLowHealth.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SAttributeComponent.h"

USBTService_CheckLowHealth::USBTService_CheckLowHealth() {
	LowHealthFraction = 0.3f;
}

void USBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	UBlackboardComponent* blackboardComp = OwnerComp.GetBlackboardComponent();
	bool bIsLowHealth; 
	if (ensure(blackboardComp))
	{
		AActor* SelfActor = Cast<AActor>(blackboardComp->GetValueAsObject("SelfActor"));
		if (SelfActor)
		{
			USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(SelfActor);
			//USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(SelfActor->GetComponentByClass(USAttributeComponent::StaticClass()));
			if (ensure(AttributeComp))
			{
				bIsLowHealth = (AttributeComp->GetHealth() / AttributeComp->GetMaxHitPoint()) < LowHealthFraction;
				//bIsLowHealth = AttributeComp->GetIsLowHealth();
				blackboardComp->SetValueAsBool(LowHealthKey.SelectedKeyName, bIsLowHealth);
			}
		}
	}
}

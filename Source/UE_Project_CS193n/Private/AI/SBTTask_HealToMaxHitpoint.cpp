// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_HealToMaxHitpoint.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type USBTTask_HealToMaxHitpoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* blackboardComp = OwnerComp.GetBlackboardComponent();
	AActor* SelfActor = Cast<AActor>(blackboardComp->GetValueAsObject("SelfActor"));
	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(SelfActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	if (AttributeComp == NULL) {
		return EBTNodeResult::Failed;
	}
	float Delta = AttributeComp->GetMaxHitPoint() - AttributeComp->GetHealth();
	AttributeComp->ApplyHealthChange(SelfActor, Delta);
	return EBTNodeResult::Succeeded;
}

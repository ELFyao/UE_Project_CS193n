// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	//Check distance between Player and AI Robot if it is out of range;
	UBlackboardComponent *blackboardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(blackboardComp))
	{
		AActor* TargetActor = Cast<AActor>(blackboardComp->GetValueAsObject("TargetActor"));
		if (TargetActor)
		{
			AAIController *AiController = OwnerComp.GetAIOwner();
			if (AiController)
			{
				APawn* Robot = AiController->GetPawn();
				if (ensure(Robot))
				{
					float DistanceToActor = FVector::Distance(TargetActor->GetActorLocation(), Robot->GetActorLocation());
					bool bWthinRange = DistanceToActor < 2000.f;
					bool bSightin = false;
					if (bWthinRange) {
						bSightin = AiController->LineOfSightTo(TargetActor);
					}
					bWthinRange = bSightin & bWthinRange;
					blackboardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, bWthinRange);
				}
			}
		}
	}
	
}

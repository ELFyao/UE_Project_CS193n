// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_RangeAttack.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SAttributeComponent.h"



USBTTask_RangeAttack::USBTTask_RangeAttack()
{
	MaxAttackSpread = 2.0f;
}



EBTNodeResult::Type USBTTask_RangeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// realize the attack function of robot 
	AAIController *MyController = OwnerComp.GetAIOwner();
	if (ensure(MyController)) {
		ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn());
		if (MyPawn == NULL) {
			return EBTNodeResult::Failed;
		}


		FVector HandsLocation = MyPawn->GetMesh()->GetSocketLocation("Muzzle_Front");
		UBlackboardComponent *blackboardComp = OwnerComp.GetBlackboardComponent();
		AActor* TargetActor = Cast<AActor>(blackboardComp->GetValueAsObject("TargetActor"));
		if (TargetActor == NULL) {
			return EBTNodeResult::Failed;
		}

		if (!USAttributeComponent::IsActorAlive(TargetActor))
		{
			return EBTNodeResult::Failed;
		}


		FVector TargetLocation = TargetActor->GetActorLocation();
		FRotator AimDirection = (TargetLocation - HandsLocation).Rotation();

		AimDirection.Pitch += FMath::RandRange(0.0f, MaxAttackSpread);
		AimDirection.Yaw += FMath::RandRange(-MaxAttackSpread, MaxAttackSpread);


		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = MyPawn;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AActor* Neo = GetWorld()->SpawnActor<AActor>(ProjectileClass, HandsLocation, AimDirection, SpawnParams);
		return Neo ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}
	return EBTNodeResult::Failed;
}


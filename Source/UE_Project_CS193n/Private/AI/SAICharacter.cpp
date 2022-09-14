// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"
#include "AIController.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PawnSenseComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSenseComp");
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSenseComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnSeePawn);

}

void ASAICharacter::OnSeePawn(APawn* Pawn)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		UBlackboardComponent *BlackBoardComp = AIC->GetBlackboardComponent();
		BlackBoardComp->SetValueAsObject("TargetActor", Pawn);
		DrawDebugString(GetWorld(), GetActorLocation(), FString::Printf(TEXT("Sense Pawn at %s"), *GetActorLocation().ToString()) ,NULL ,FColor::White, 5.0f, true);
	}
}

// Called when the game starts or when spawned


// Called to bind functionality to input



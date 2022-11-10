// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"
#include "AIController.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SAttributeComponent.h"
#include "BrainComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SWorldUserWidget.h"


// Sets default values
ASAICharacter::ASAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PawnSenseComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSenseComp");
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;


	TimeToHitParams = "TimeToHit";
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSenseComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnSeePawn);
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::onHealthChanged);
}

void ASAICharacter::Death()
{
	Destroy();
}

void ASAICharacter::onHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwingComp, float NewHealth, float Dealta)
{
	//Handle Once the actor was dead.
	if (InstigatorActor != this)
	{
		SetTargetActor(InstigatorActor);
	}

	if (!IsValid(ActiveHealthBar))
	{
		ActiveHealthBar = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);

		ActiveHealthBar->AttachActor = this;
		ActiveHealthBar->AddToViewport();      //when we call this, would call USWorldUserWidget.construct()
	}

	GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParams, GetWorld()->TimeSeconds);


	if (NewHealth <= 0)
	{
		// Behavior Tree End
		// Rag doll
		AAIController* AIC = Cast<AAIController>(GetController());
		if (ensure(AIC)) {
			AIC->GetBrainComponent()->StopLogic("Killed");
		}
		GetMesh()->SetAllBodiesSimulatePhysics(true);
		GetMesh()->SetCollisionProfileName("Ragdoll");

		GetCharacterMovement()->DisableMovement();
		SetLifeSpan(5);
		GetWorldTimerManager().SetTimer(TimerHandle_Death, this, &ASAICharacter::Death,5.0f);
	}
}

void ASAICharacter::SetTargetActor(AActor* InstigatorActor)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", InstigatorActor);
	}
}

void ASAICharacter::OnSeePawn(APawn* Pawn)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		SetTargetActor(Pawn);
		DrawDebugString(GetWorld(), GetActorLocation(), FString::Printf(TEXT("Sense Pawn at %s"), *GetActorLocation().ToString()) ,NULL ,FColor::White, 5.0f, true);
	}
}

// Called when the game starts or when spawned


// Called to bind functionality to input



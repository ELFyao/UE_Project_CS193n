// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SGameplayInterface.h"
#include "DrawDebugHelpers.h"
#include "SCVarObject.h"
#include "../Public/SWorldUserWidget.h"



// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	TraceDistance = 500.0f;
	TraceRadius = 30.0f;
	CollisionChannel = ECC_WorldDynamic;
	// ...
}

// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	APawn* pawn = Cast<APawn>(GetOwner());
	if (pawn->IsLocallyControlled()) {
		FindBestInteractable();
	}
}

void USInteractionComponent::FindBestInteractable()
{
	FCollisionObjectQueryParams ObjectQueryParams;

	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	AActor* MyOwner = GetOwner();

	FVector Eyelocation;
	FRotator EyeRotator;
	MyOwner->GetActorEyesViewPoint(Eyelocation, EyeRotator);

	FVector End = Eyelocation + (EyeRotator.Vector() * TraceDistance);

	//FHitResult Hit;

	//bool bBlockinghit = GetWorld()->LineTraceSingleByObjectType(Hit, Eyelocation, End, ObjectQueryParams);
	TArray<FHitResult> Hits;

	//float Radius = 200.0f;
	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);
	bool bBlockinghit = GetWorld()->SweepMultiByObjectType(Hits, Eyelocation, End, FQuat::Identity, ObjectQueryParams, Shape);
	FColor LineColor = bBlockinghit ? FColor::Green : FColor::Red;

	FocusActor = nullptr;

	for (FHitResult Hit : Hits)
	{
		AActor* HitResult = Hit.GetActor();
		if (HitResult)
		{
			if (HitResult->Implements<USGameplayInterface>())                          // 注意这里是USGameplayInterface 不是ISGameplayInterface。虽然unreal默认interface会生成两个类																//似乎第一个U代表宏，起一个定义的作用,而I是在code中起实现的作用。
			{
				FocusActor = HitResult;
				break;
			}
		}
		if (CVarDrawDebugShape.GetValueOnGameThread())
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, 32, LineColor, false, 2.0f);
		}
	}

	if (FocusActor)
	{
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<USWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachActor = FocusActor;
			
			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}

	}
	else
	{
		if (DefaultWidgetInstance) 
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
		
	}

	if (CVarDrawDebugShape.GetValueOnGameThread())
	{
		DrawDebugLine(GetWorld(), Eyelocation, End, FColor::Red, false, 2.0f, 0, 2.0f);
	}
}


void USInteractionComponent::ServerInteract_Implementation(AActor* InFocus)
{
	if (InFocus == nullptr) {
		GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Red, "No Focus Actor to Interact");
		return;
	}
	APawn* MyApawn = Cast<APawn>(GetOwner());
	ISGameplayInterface::Execute_Interact(InFocus, MyApawn);
}

void USInteractionComponent::PrimaryInteract()
{
	ServerInteract(FocusActor);
}
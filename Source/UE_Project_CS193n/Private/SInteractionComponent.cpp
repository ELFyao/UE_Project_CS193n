// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SGameplayInterface.h"
#include "DrawDebugHelpers.h"
#include "SCVarObject.h"


void USInteractionComponent::PrimaryInteract()
{
	FCollisionObjectQueryParams ObjectQueryParams;

	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* MyOwner = GetOwner();

	FVector Eyelocation;
	FRotator EyeRotator;
	MyOwner->GetActorEyesViewPoint(Eyelocation, EyeRotator);

	FVector End = Eyelocation + (EyeRotator.Vector() * 1000.f);

	//FHitResult Hit;

	//bool bBlockinghit = GetWorld()->LineTraceSingleByObjectType(Hit, Eyelocation, End, ObjectQueryParams);
	TArray<FHitResult> Hits;

	float Radius = 200.0f;
	FCollisionShape Shape;
	Shape.SetSphere(Radius);
	bool bBlockinghit = GetWorld()->SweepMultiByObjectType(Hits, Eyelocation, End, FQuat::Identity, ObjectQueryParams, Shape);
	FColor LineColor = bBlockinghit ? FColor::Green : FColor::Red;
	for (FHitResult Hit : Hits)
	{
		AActor* HitResult = Hit.GetActor();
		if (HitResult)
		{
			if (HitResult->Implements<USGameplayInterface>())                          // 注意这里是USGameplayInterface 不是ISGameplayInterface。虽然unreal默认interface会生成两个类																//似乎第一个U代表宏，起一个定义的作用,而I是在code中起实现的作用。
			{
				APawn* MyApawn = Cast<APawn>(MyOwner);
				ISGameplayInterface::Execute_Interact(HitResult, MyApawn);
				break;
			}
		}
		if (CVarDrawDebugShape.GetValueOnGameThread())
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, LineColor, false, 2.0f);
		}
	}
	if (CVarDrawDebugShape.GetValueOnGameThread())
	{
		DrawDebugLine(GetWorld(), Eyelocation, End, FColor::Red, false, 2.0f,0,2.0f);
	}
}

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

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
}


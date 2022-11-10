// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class UPawnSensingComponent;
class USAttributeComponent;
class UUserWidget;
class USWorldUserWidget;

UCLASS()
class UE_PROJECT_CS193N_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASAICharacter();


protected:

	USWorldUserWidget* ActiveHealthBar;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSenseComp;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USAttributeComponent* AttributeComp;
	
	virtual void PostInitializeComponents() override;

	void Death();

	FTimerHandle TimerHandle_Death;


	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParams;


	UFUNCTION()
	void onHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwingComp, float NewHealth, float Dealta);

	void SetTargetActor(AActor* InstigatorActor);

	UFUNCTION()
	void OnSeePawn(APawn* Pawn);
};

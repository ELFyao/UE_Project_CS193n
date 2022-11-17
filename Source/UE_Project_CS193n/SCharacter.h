// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USInteractionComponent;
class UAnimMontage;
class USAttackComponent;
class ASProjectileParent;
class USAttributeComponent;
class USActionComponent;
class USAction;

UCLASS()
class UE_PROJECT_CS193N_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Actions")
	TArray<TSubclassOf<USAction>> ActionClasses;


	UPROPERTY(VisibleAnywhere)
	USInteractionComponent *InteractionComp;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParams;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USActionComponent *ActionComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USAttributeComponent* AttributeComp;

	/*Movement Function*/
	void MoveForward(float value);
	void MoveRight(float value);
	void JumpStart();
	void JumpEnd();

	void StartSprint();
	void StopSprint();




	/*Interact Function*/
	void PrimaryInteract();


	/*Attack Function*/
	void PrimaryAttack();

	void PrimaryDash();


	void BlackHoleAttack();

	UFUNCTION()
	void onHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwingComp, float NewHealth, float Dealta);

	virtual void PostInitializeComponents() override;

	virtual FVector GetPawnViewLocation() const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(EXEC)
	void HealSelf(float Amount = 100);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

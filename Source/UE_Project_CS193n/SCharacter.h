// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USInteractionComponent;
class UAnimMontage;

UCLASS()
class UE_PROJECT_CS193N_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();
	
protected:
	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<AActor> ProjectileClass;
	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> BlackholeClass;

	FTimerHandle TimerHandle_PrimaryAttack;
	//FTimerHandle TimerHandle_BlackholeAttack;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(visibleAnywhere)
	USInteractionComponent *InteractionComp;
	

	UPROPERTY(visibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(visibleAnywhere)
	USpringArmComponent* SpringArmComp;


	/*Movement Function*/
	void MoveForward(float value);
	void MoveRight(float value);
	void JumpStart();
	void JumpEnd();

	/*Interact Function*/
	void PrimaryInteract();



	/*Attack Function*/
	void PrimaryAttack();
	void PrimaryAttack_TimeElapsed();
	//void BlackHoleAttack();
	//void BlackholeAttack_TimeElapsed();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

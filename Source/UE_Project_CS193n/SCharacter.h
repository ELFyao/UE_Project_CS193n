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

UCLASS()
class UE_PROJECT_CS193N_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HandSocketName;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParams;

	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<AActor> ProjectileClass;
	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> BlackholeClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> DashClass;


	float AttackDistance;

	FTimerHandle TimerHandle_PrimaryAttack;
	FTimerHandle TimerHandle_BlackholeAttack;
	FTimerHandle TimerHandle_Dash;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* HandFlashVFX;

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent *InteractionComp;

	UPROPERTY(VisibleAnywhere)
	USAttackComponent *AttackComp;
	


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

	/*Interact Function*/
	void PrimaryInteract();

	void SpawnProjectile(TSubclassOf<AActor> ClasstoSpawn);

	/*Attack Function*/
	void PrimaryAttack();
	void PrimaryAttack_TimeElapsed();

	void PrimaryDash();
	void PrimaryDash_TimeElapsed();


	void BlackHoleAttack();
	void BlackholeAttack_TimeElapsed();

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

// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include"GameFramework/SpringArmComponent.h"
#include"GameFramework/CharacterMovementComponent.h"
#include "SAttributeComponent.h"
#include "SInteractionComponent.h"
#include "SAttackComponent.h"
#include"DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "SActionComponent.h"
#include "SAction.h"
#include "SCVarObject.h"
// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CamreaComp");
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");
	//AttackComp = CreateDefaultSubobject<USAttackComponent>("AttackComp");

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");
	
	CameraComp->SetupAttachment(SpringArmComp);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	bUseControllerRotationYaw = false;
	TimeToHitParams = "TimeToHit";
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
}



// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// -- Rotation Visualization -- //

	if (CVarDrawDebugShape.GetValueOnGameThread())
	{
		const float DrawScale = 100.0f;
		const float Thickness = 5.0f;

		FVector LineStart = GetActorLocation();
		// Offset to the right of pawn
		LineStart += GetActorRightVector() * 100.0f;
		// Set line end in direction of the actor's forward
		FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.0f);
		// Draw Actor's Direction
		DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, DrawScale, FColor::Yellow, false, 0.0f, 0, Thickness);

		FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);
		// Draw 'Controller' Rotation ('PlayerController' that 'possessed' this character)
		DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, DrawScale, FColor::Green, false, 0.0f, 0, Thickness);
	}
}

void ASCharacter::HealSelf(float Amount /*= 100*/)
{
	AttributeComp->ApplyHealthChange(this, Amount);
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("BlackholeAttack", IE_Pressed, this, &ASCharacter::BlackHoleAttack);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::JumpStart);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ASCharacter::JumpEnd);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ASCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ASCharacter::PrimaryDash);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASCharacter::StartSprint);

	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASCharacter::StopSprint);



}


void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::onHealthChanged);


}

void ASCharacter::PrimaryAttack()
{
	ActionComp->StartActionByName(this, "PrimaryAttack");
}



void ASCharacter::PrimaryDash()
{
	ActionComp->StartActionByName(this, "PrimaryDash");
}




void ASCharacter::BlackHoleAttack()
{
	ActionComp->StartActionByName(this, "BlackHole");
}






void ASCharacter::onHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwingComp, float NewHealth, float Dealta)
{
	if (Dealta < 0) {
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParams, GetWorld()->TimeSeconds) ;
		if (NewHealth <= 0)
		{
			APlayerController* PC = Cast<APlayerController>(GetController());
			DisableInput(PC);
		}
	}
// 	if (NewHealth <= 0 && Dealta < 0) {
// 
// 	}
}




FVector ASCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
}



void ASCharacter::MoveForward(float value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	AddMovementInput(ControlRot.Vector(), value);
}

void ASCharacter::MoveRight(float value)
{
	//FVector rightVector = GetActorRightVector();
	//FVector forwardVector = GetActorForwardVector();
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	FVector RightVec =FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVec, value);
}

void ASCharacter::JumpStart()
{
	bPressedJump = true;
}

void ASCharacter::JumpEnd()
{
	bPressedJump = false;
}

void ASCharacter::StartSprint()
{
	ActionComp->StartActionByName(this, "Sprint");

}

void ASCharacter::StopSprint()
{
	ActionComp->StopActionByName(this, "Sprint");
}

void ASCharacter::PrimaryInteract()
{
	InteractionComp->PrimaryInteract();
}






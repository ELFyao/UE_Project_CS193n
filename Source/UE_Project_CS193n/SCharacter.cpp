// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include"GameFramework/SpringArmComponent.h"
#include"GameFramework/CharacterMovementComponent.h"
#include "SAttributeComponent.h"
#include "SInteractionComponent.h"
#include "SAttackComponent.h"
#include"DrawDebugHelpers.h"

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
	AttackComp = CreateDefaultSubobject<USAttackComponent>("AttackComp");

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
	
	CameraComp->SetupAttachment(SpringArmComp);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	bUseControllerRotationYaw = false;

	AttackDistance = 5000.f;

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


}

void ASCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.2f);

	// if something goes wrong like character died; 
	//GetWorldTimerManager().ClearTimer(TimerHandle_PrimaryAttack);
}

void ASCharacter::PrimaryAttack_TimeElapsed()
{
	SpawnProjectile(ProjectileClass);
}



void ASCharacter::PrimaryDash()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryDash_TimeElapsed, 0.2f);
}

void ASCharacter::PrimaryDash_TimeElapsed()
{
	SpawnProjectile(DashClass);
}


void ASCharacter::BlackHoleAttack()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::BlackholeAttack_TimeElapsed, 0.2f);
}


void ASCharacter::BlackholeAttack_TimeElapsed()
{
	SpawnProjectile(BlackholeClass);
}




void ASCharacter::onHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwingComp, float NewHealth, float Dealta)
{
	if (NewHealth <= 0 && Dealta < 0) {
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);
	}
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::onHealthChanged);
}

void ASCharacter::SpawnProjectile(TSubclassOf<AActor> ClasstoSpawn)
{
	if (ensure(ClasstoSpawn))
	{
		FVector handsLocation = GetMesh()->GetSocketLocation("Muzzle_01");

		/*
		* assignment_2 part3 start here.
		*/

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;



		// line trace
		FVector ControlLocation = CameraComp->GetComponentLocation();
		FRotator ControlRotation = GetControlRotation();

		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);      //这里由ECC_PhysicsBody 改成了ECC_Pawn
		FVector End = ControlLocation + (ControlRotation.Vector() * AttackDistance);

		FHitResult Hit;

		FVector AttackEnd;

		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		//bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, ControlLocation, End, ObjectQueryParams);
		bool bBlockingHit = GetWorld()->SweepSingleByObjectType(Hit, ControlLocation, End, FQuat::Identity, ObjectQueryParams, Shape, Params);
		if (bBlockingHit) {
			AttackEnd = Hit.ImpactPoint;
		}
		else
		{
			AttackEnd = End;
		}

		FRotator AttackDirection = (AttackEnd - handsLocation).Rotation();
		FTransform SpawnTM = FTransform(AttackDirection, handsLocation);
		GetWorld()->SpawnActor<AActor>(ClasstoSpawn, SpawnTM, SpawnParams);
	}
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

void ASCharacter::PrimaryInteract()
{
	InteractionComp->PrimaryInteract();
}






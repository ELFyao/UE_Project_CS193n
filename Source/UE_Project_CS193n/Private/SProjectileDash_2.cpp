// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectileDash_2.h"

ASProjectileDash_2::ASProjectileDash_2()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASProjectileDash_2::ProjectileEffect(APawn* InstigatorPawn)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void ASProjectileDash_2::ProjectileAttack_Implementation(APawn* InstigatorPawn)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void ASProjectileDash_2::BeginPlay()
{
	Super::BeginPlay();
}

void ASProjectileDash_2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//void ASProjectileDash_2::ProjectileEffect()
//{
//
//}

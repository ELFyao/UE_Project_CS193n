// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SCVarObject.generated.h"

/**
 * 
 */
static TAutoConsoleVariable<bool> CVarDrawDebugShape(TEXT("su.DrawDebugShape"), false, TEXT("Enable Drawing Debug Shape When Interacting with something"), ECVF_Cheat);

UCLASS()
class UE_PROJECT_CS193N_API USCVarObject : public UObject
{
	GENERATED_BODY()
};

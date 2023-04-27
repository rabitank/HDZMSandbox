// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HGameModeFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class HDZMSANDBOX_API UHGameModeFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	static bool ApplyEnergy(AActor* CauserActor, AActor* TargetActor, float EnergyAmount);

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	static bool ApplyDirectionalEnergy(AActor* CauserActor, AActor* TargetActor, float EnergyAmount, const FHitResult& hit);
};

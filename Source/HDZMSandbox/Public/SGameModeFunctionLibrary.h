// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SGameModeFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class HDZMSANDBOX_API USGameModeFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		static bool ApplyDamage(AActor* CauserActor, AActor* TargetActor, float DamageAmount);

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		static bool ApplyDirectionalDamage(AActor* CauserActor, AActor* TargetActor, float DamageAmount, const FHitResult& hit);



};

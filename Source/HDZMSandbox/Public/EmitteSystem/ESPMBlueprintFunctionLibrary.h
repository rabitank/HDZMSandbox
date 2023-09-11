// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ESPMBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
class AHEmitterPattern;
class AHEmitter;
class AHPlayerCharacter;

UCLASS()
class HDZMSANDBOX_API UESPMBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable,Category = "ESPM System")
	static bool AddRecoilByEmittePattern( FName emittePattern , AHEmitter* emitter, AHPlayerCharacter* target );
	
	

	/// <summary>
	/// ����ֻ��emittePattern����Ҫ��������,���￼�ǵ��Ժ�emittePatttern���ػ�����ָ��;
	/// </summary>
	UFUNCTION(BlueprintCallable,Category = "ESPM System")
	static bool HasEnoughOneShootEnergy( AHEmitterPattern* emittePattern , AHEmitter* emitter, AActor* target );
	
	UFUNCTION(BlueprintCallable,Category = "ESPM System")
	static bool ApplyEmittePatternEnergy( AHEmitterPattern* emittePattern , AHEmitter* emitter, AActor* target );

	UFUNCTION(BlueprintCallable, Category = "ESPM System")
	static bool ApplyEnergy(AActor* CauserActor, AActor* TargetActor, float DamageAmount);

	UFUNCTION(BlueprintCallable,Category = "ESPM System")
	static bool ApplyDirectionalEnergy(AActor* CauserActor, AActor* TargetActor, float EnergyAmount, const FHitResult& hit);

};
//OwnerEmitter->GetOwner()

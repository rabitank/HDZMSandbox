// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HBulletBase.h"
#include "HEmitInterface.h"
#include "HEmitAction.h"
#include "Delegates/DelegateCombinations.h"
#include "HEmitterCoreBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnShootingStateChanged, class UHEmitterCoreBase*, thisCore, class AActor*, instigatorActor);

/**
 * 
 */
UCLASS()
class HDZMSANDBOX_API UHEmitterCoreBase : public UObject, public IHEmitInterface
{
	GENERATED_BODY()
public:
	//************************************
	// Method:    UHEmitterCoreBase
	// Access:    public 
	// Desribe:	  Dont't allow Directly instant UHEmitterCoreBase in UE
	//************************************
	UHEmitterCoreBase();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "EmitterCore")
		TSubclassOf<AHBulletBase> BulletClass;
	
	UPROPERTY(VisibleAnywhere, Category = "EmitterCore")
	class UHAttributeComponent* ComOwnerEnergySource;
	
	UPROPERTY(VisibleAnywhere, Category = "EmitterCore")
	class UHEmitterComponent* ComOwnerEmitter;

	

	UPROPERTY(EditDefaultsOnly, Category = "EmitterCore")
	float CoolDownDuration;

	UPROPERTY(VisibleAnywhere,Category = "EmitterCore")
	float LastShootTime;
	
	UPROPERTY(EditDefaultsOnly,Category = "EmitterCore")
	float SpawnDuration;
	
	UPROPERTY(EditDefaultsOnly,Category = "EmitterCore")
	float BackInitDuration;

	UPROPERTY(EditDefaultsOnly, Category = "EmitterCore")
	float EnergyDemand;

	UPROPERTY(EditDefaultsOnly, Category = "EmitterCore")
	bool bIsAbsorbable;
	
	UPROPERTY(EditDefaultsOnly, Category = "EmitterCore")
	bool bIsShooting;

	//should spawn bullet before UHEmitActions preparetime
	FTimerHandle TimerHandle_SpawnBulletDelay;

	UFUNCTION(BlueprintNativeEvent)
	void SpawnBulletDelay(AActor* instigator );

	FTimerHandle TimeHandle_PrepareOverStopShooting;

public:


	void Initialize(UHEmitterComponent* EmitterComp) ;

	void Emit_Implementation(AActor* Instigator);

	UPROPERTY(EditDefaultsOnly, Category = "EmitterCore")
		FName CoreName;

	UFUNCTION(BlueprintCallable)
	bool CanEmit(AActor* Instigator);



	UFUNCTION(BlueprintNativeEvent)
	void OnPressedTrigger(AActor* Instigator) ;

	UFUNCTION(BlueprintNativeEvent)
	void OnReleasedTriger(AActor* Instiagtor);

	UFUNCTION(BlueprintCallable)
		bool IsShooting() const { return bIsShooting; }

	UFUNCTION(BlueprintNativeEvent)
	void StopShoot(AActor* Instigator) ;

	UFUNCTION(BlueprintNativeEvent)
	void StartShoot(AActor* Instiagtor);

	FOnShootingStateChanged OnShootStarted;
	FOnShootingStateChanged OnShootStoped;



};



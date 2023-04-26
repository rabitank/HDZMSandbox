// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HBulletBase.h"
#include "HEmitInterface.h"
#include "HEmitAction.h"
#include "HEmitterCoreBase.generated.h"

/**
 * 
 */
UCLASS()
class HDZMSANDBOX_API UHEmitterCoreBase : public UHEmitAction , public IHEmitInterface
{
	GENERATED_BODY()
public:
	UHEmitterCoreBase();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "EmitterCore")
		TSubclassOf<AHBulletBase> BulletClass;
	
	UPROPERTY(EditAnywhere, Category = "EmitterCore")
	class UHAttributeComponent* ComEnergySource;


	UPROPERTY(EditDefaultsOnly, Category = "EmitterCore")
	float CoolDownDuration;

	UPROPERTY(VisibleAnywhere,Category = "EmitterCore")
	float LastShootTime;
	
	UPROPERTY(EditDefaultsOnly,Category = "EmitterCore")
	float SpawnDuration;
	
	UPROPERTY(EditDefaultsOnly,Category = "EmitterCore")
	float BackInitDuration;

	UPROPERTY(EditDefaultsOnly, Category = "EmitterCore")
	bool bIsAbsorbable;

	UPROPERTY(EditDefaultsOnly, Category = "EmitterCore")
	float EnergyDemand;

	//should spawn bullet before UHEmitActions preparetime
	FTimerHandle TimerHandle_SpawnBulletDelay;

	UFUNCTION(BlueprintNativeEvent)
	void SpawnBulletDelay(AActor* instigator );

	FTimerHandle TimeHandle_PrepareOverStopShooting;

public:

	void Initialize(UHAttributeComponent* energySource, UHEmitterComponent* EmitterComp) ;

	void Emit_Implementation(AActor* Instigator);


	UPROPERTY(EditDefaultsOnly, Category = "EmitterCore")
		FName CoreName;

	UFUNCTION(BlueprintCallable)
	bool CanEmit(AActor* Instigator);

	virtual void StopShoot_Implementation(AActor* Instigator) override;

	virtual void StartShoot_Implementation(AActor* Instiagtor) override;



};



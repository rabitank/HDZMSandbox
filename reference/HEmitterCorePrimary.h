// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HEmitterCoreBase.h"
#include "HEmitterCorePrimary.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class HDZMSANDBOX_API UHEmitterCorePrimary : public UHEmitterCoreBase
{

	GENERATED_BODY()
	//singal Bullet pre Trigger. 
	//: 单发



protected:
	UPROPERTY(EditAnywhere, Category = "EmitterCore")
	float TriggerTimeDuration;

	UPROPERTY()
	FTimerHandle TimeHandle_StartShoot;
	
	UPROPERTY()
	FTimerHandle TimeHandle_StopShoot;

public:
	UHEmitterCorePrimary();

	virtual bool OnPressedTrigger_Implementation(AActor* Instigator) override;
	virtual	void OnReleasedTriger_Implementation(AActor* Instiagtor) override;
	virtual	void StopShoot_Implementation(AActor* Instigator);
	virtual void StartShoot_Implementation(AActor* Instiagtor);

};

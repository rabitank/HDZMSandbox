// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SActionEffect.h"
#include "SActionEffect_Thorns.generated.h"

/**
 * 
 */
UCLASS()
class HDZMSANDBOX_API USActionEffect_Thorns : public USActionEffect
{
	GENERATED_BODY()
protected:
	UFUNCTION()
	void OnHealthChanged(class USAttributeComponent* owningComp, AActor* instigatorActor, float newHealth, float delta);

	UPROPERTY(EditDefaultsOnly, Category = "DamageControl", meta = (ClampMax = 1,ClampMin= 0))
	float ThornsPercent;

public:
	USActionEffect_Thorns();

	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator) override;


};

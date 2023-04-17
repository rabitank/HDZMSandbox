// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerUpActor.h"
#include "SAction.h"
#include "SPowerUp_Action.generated.h"

/**
 * 
 */
UCLASS()
class HDZMSANDBOX_API ASPowerUp_Action : public ASPowerUpActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "PowerUp")
		TSubclassOf<USAction> ContainActionClass;


	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	UFUNCTION(NetMulticast, Reliable)
		void MultiCastAddAction( class USActionComponent* actions);

	
};

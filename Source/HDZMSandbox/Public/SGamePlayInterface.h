// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SGamePlayInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USGamePlayInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HDZMSANDBOX_API ISGamePlayInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void Interact(APawn* InstigatorPawn);
	//BlueprintNativeEvent:you can imp interface in c++ and bp;
	//if inplement bpfunction will override cpp func. but you can call parent Interact to call cpp func

	//write implementation:
	//1. inherit interface
	//2. override functionName_Implementation();

};

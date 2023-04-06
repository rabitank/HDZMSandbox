// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

UCLASS()
class HDZMSANDBOX_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASAICharacter();

protected:
	UFUNCTION()
	void SetTargetActor(AActor* target);


	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UPawnSensingComponent* ComPawnSense;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USAttributeComponent* ComSAttribute;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category = "Effects")
	FName HitTimeParametersName;



	UFUNCTION(BlueprintNativeEvent)
	void OnHealthChanged(USAttributeComponent* owningComp, AActor* instigatorActor, float newHealth, float delta);


	virtual void PostInitializeComponents() override;

	UFUNCTION()
		void OnSeePawn(APawn* Pawn);


};

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

	class USWorldUserWidget* ActiveHealthBar;

	UFUNCTION()
	void SetTargetActor(AActor* target);

	UFUNCTION()
		AActor* GetTargetActor();

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UPawnSensingComponent* ComPawnSense;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USActionComponent* ComSAction;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USAttributeComponent* ComSAttribute;


	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category = "Effects")
	FName TargetActorKey;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category = "Effects")
	FName HitTimeParametersName;
	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	TSubclassOf <class UUserWidget> AIHealthWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	TSubclassOf <class UUserWidget> AIFindPlayerWidgetClass;
	

	UFUNCTION(BlueprintNativeEvent)
	void OnHealthChanged(USAttributeComponent* owningComp, AActor* instigatorActor, float newHealth, float delta);


	virtual void PostInitializeComponents() override;
	UFUNCTION(NetMulticast, Unreliable)
		void MultiCastCreateFindWidget();

	UFUNCTION()
		void OnSeePawn(APawn* Pawn);


};

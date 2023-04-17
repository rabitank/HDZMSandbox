// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USInteractionComponent;

UCLASS(Blueprintable)
class HDZMSANDBOX_API ASCharacter : public ACharacter
{	
	GENERATED_BODY()

		//you can directly change the macro of GENERATED_BODY in class.generated.h , so to change the line it should be.
protected:
	

	// override the  eyetrace:GetActorEyesViewPoint() method
	virtual FVector GetPawnViewLocation() const override;



public:
	// Sets default values for this character's properties
	ASCharacter();
protected:

	UPROPERTY(VisibleAnywhere, Category = "Effects")
		FName HitTimeParametersName;

	UPROPERTY(EditAnywhere, Category = "Effects")
		TSubclassOf<UCameraShakeBase> CamerShakeDamaged;

	UPROPERTY(VisibleDefaultsOnly)
	USpringArmComponent* ComSpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* ComCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USInteractionComponent* ComSInteraction;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USActionComponent* ComActions;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	class USAttributeComponent* ComSAttribute;


	UFUNCTION()
	void OnHealthChanged(USAttributeComponent* owningComp, AActor* instigatorActor, float newHealth, float delta);

	virtual  void PostInitializeComponents() override;

private:

	void MoveForward(float Val);
	void MoveRight(float Val);
	
	void PrimaryInteraction();
	void PrimaryAttack();

	void BlackholeAttack();

	void Dash();

	//Sprint:rush? Excute by ComAction
	void SprintStart();
	void SprintStop();

protected:
	//virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

	//Exec: cmd funciton/command 
	//for debug and cheat
	UFUNCTION(Exec)
		void HealSelf(float amount=100.f);


};

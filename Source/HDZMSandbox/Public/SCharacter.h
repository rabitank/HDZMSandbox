// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USInteractionComponent;

UCLASS()
class HDZMSANDBOX_API ASCharacter : public ACharacter
{	
	GENERATED_BODY()

		//you can directly change the macro of GENERATED_BODY in class.generated.h , so to change the line it should be.
protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> PorjectileClass;

public:
	// Sets default values for this character's properties
	ASCharacter();


protected:

	UPROPERTY(VisibleDefaultsOnly)
	USpringArmComponent* ComSpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ComCamera;

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* ComSInteraction;


private:

	void MoveForward(float Val);
	void MoveRight(float Val);
	
	void PrimaryAttack();
	void PrimaryInteraction();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

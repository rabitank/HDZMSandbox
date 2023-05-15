// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HBulletBase.h"
#include "HPlayerCharacter.generated.h"

UCLASS()
class HDZMSANDBOX_API AHPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHPlayerCharacter();

protected:
	UPROPERTY( EditAnywhere,BlueprintReadWrite, Category = "Anim Control")
		bool bCrouchButtonDown;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Anim Control")
		bool bJumpButtonDown;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Emitter")
		float EmitterMoveRadiance;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Emitter")
		FVector EmitterMoveOffSet;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Emitter")
		FRotator EmitterDirectionOffSet;


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category = "Components")
		class USpringArmComponent* ComSpringArm;
	

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UCameraComponent* ComCamera;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UHActionComponent* ComActions;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UHAttributeComponent* ComAttribute;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UHEmitterComponent* ComEmitter;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
		class USphereComponent* ComSphereCollision;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
		class USceneComponent* ComEmitterLocation;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
		class USphereComponent* ComEmitterMoveController;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UArrowComponent* ComEmitterDireation;

protected:
	void MoveRight(float val);
	void MoveForward(float val);
	void ChangeCore(float val);

	void StartCrouch() { bCrouchButtonDown= true; };
	void StopCrouch() { bCrouchButtonDown = false; };

	void StartJump();
	void StopJump();
	void DispatchJumpData();
	void OnTriggerPressed();
	void OnTriggerReleased();

	void OnStopedRestoreEnergy();
	void OnStartRestoreEnergy();


public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;
	
	USceneComponent* GetEmitterMoveComp() { return ComEmitterLocation; };

};

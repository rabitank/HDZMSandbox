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
	UPROPERTY(BlueprintReadWrite, Category = "Anim Control")
		bool bCrouchButtonDown;
	
	UPROPERTY(BlueprintReadWrite, Category = "Anim Control")
		bool bJumpButtonDown;

	int64 RestoreEnergyKeyPressedTick =0;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class USpringArmComponent* ComSpringArm;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UCameraComponent* ComCamera;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UHAttributeComponent* ComAttribute;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UHEmitterComponent* ComEmitter;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
		class USphereComponent* ComSphereCollision;
	

	UPROPERTY(EditDefaultsOnly, Category = "Bullet")
		TSubclassOf<AHBulletBase> BulletClass;


protected:
	void MoveRight(float val);
	void MoveForward(float val);
	void ChangeCore(float val);

	void StartCrouch() { bCrouchButtonDown= true; };
	void StopCrouch() { bCrouchButtonDown = false; };

	void StartJump();
	void StopJump();
	void DispatchJumpData();
	void PrimaryEmitt();

	void OnRestoreEnergy();
	void OnStartRestoreEnergy();


public:	
	// Called every fram
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HBulletBase.h"
#include "State/HShootType.h"
#include "AlsCharacter.h"
#include "AlsCameraComponent.h"
#include "HPlayerCharacter.generated.h"

UCLASS(Blueprintable)
class HDZMSANDBOX_API AHPlayerCharacter : public AAlsCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHPlayerCharacter();

private:

	FTimerHandle SprintStartTimer;


protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
		class UAlsCameraComponent* ComAlsCamera;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UHActionComponent* ComActions;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UHAttributeComponent* ComAttribute;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class USphereComponent* ComSphereCollision;


	UPROPERTY(BlueprintReadWrite, Category = "EmitterControl")
		EHShootType ShootType{EHShootType::None};
	
	UPROPERTY(BlueprintReadWrite, Category = "EmitterControl")
		bool bEmitterAiming{false};
	




protected:
	void MoveRight(float val);
	void MoveForward(float val);

	void OnLookUp(float val);
	void OnLookRight(float val);

	void OnTriggerPressed();
	void OnTriggerReleased();

	void OnStopedRestoreEnergy();
	void OnStartRestoreEnergy();

	void OnAimPressed();
	void OnAimReleased();



	void OnWalk();
	void OnCrouch();
	void OnJumpPressed();
	void OnJumpReleased();

	void OnSprintPressed();
	void OnSprintReleased();
	void OnRoll();

	virtual void CalcCamera (const float DeltaTiem, FMinimalViewInfo& ViewInfo) override;

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	virtual void DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& Unused, float& VerticalPosition) override;
public:
	inline EHShootType GetShootType() const { return ShootType; };
	inline bool GetIsEmitterAiming() const { return bEmitterAiming; };


};

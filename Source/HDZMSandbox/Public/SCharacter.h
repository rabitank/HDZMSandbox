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
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> MagicProjectile;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> BlackholeClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	class UAnimMontage* AttackAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	class UParticleSystem* ThrowMagicProEffect;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<class UCameraShakeBase> CamerShake;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<AActor> DashPorClass;

	UPROPERTY(EditAnywhere, Category = "Skill")
	class UAnimMontage* DashAnim;
	
	UPROPERTY(EditAnywhere, Category = "Skill")
	float DashDistance;

	
	FTimerHandle TimeHandle_PrimaryAttack;
	FTimerHandle TimeHandle_Dash;


public:
	// Sets default values for this character's properties
	ASCharacter();


protected:

	UPROPERTY(VisibleDefaultsOnly)
	USpringArmComponent* ComSpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* ComCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USInteractionComponent* ComSInteraction;
	
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
	void PrimaryAttack_Elapsed();

	void BlackholeAttack();

	void Dash();
	void Dash_Elapsed();

	void SpawnProjectile(TSubclassOf<AActor> projectileType );

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

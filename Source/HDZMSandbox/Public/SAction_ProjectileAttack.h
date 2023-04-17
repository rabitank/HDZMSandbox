// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SAction_ProjectileAttack.generated.h"

/**
 * 
 */
UCLASS()
class HDZMSANDBOX_API USAction_ProjectileAttack : public USAction
{
	GENERATED_BODY()
public:
	virtual void StartAction_Implementation(AActor* Instigator) override;
	USAction_ProjectileAttack();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Effects")
		FName HandSocketName;
	
	UPROPERTY(EditAnywhere, Category = "Effects")
	float CostRage;

	UPROPERTY(EditAnywhere, Category = "Attack")
		TSubclassOf<AActor> MagicProjectile;

	UPROPERTY(EditAnywhere, Category = "Attack")
		class UAnimMontage* AttackAnim;
	
	UPROPERTY(EditAnywhere, Category = "Effects")
		TSubclassOf<UCameraShakeBase> CamerShakeAttack;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackDelay ;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	class UParticleSystem* ThrowMagicProEffect;

	UFUNCTION()
		void AttackDelay_Elapsed(ACharacter* Instigator);



};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HBulletBase.h"
#include "HRegularBullet.generated.h"

/**
 * 
 */
UCLASS()
class HDZMSANDBOX_API AHRegularBullet : public AHBulletBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet | Movement")
	float LinearVelocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet | Movement")
	float LinearAcceleration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet | Movement")
	float MaxLinearVeclocity;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Bullet | Movement")
	float AngularVelocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet | Movement")
	FVector AngularAxis {-1.f,0.f,0.f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet | Movement")
	float AngularAcceleration{0.f};

	
	virtual void UpdateVelcoity(float DeltaTime);
	virtual void UpdateAccelreation(float DeltaTime) {};
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	//	class UParticleSystemComponent* ComEffectParticle;
	//
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	//	class UParticleSystem* ExplodeParticle;

public:		
	inline void SetLinearVelocity(float LV) { LinearVelocity = LV; };
	inline void SetLinearAcceleration(float LA) { LinearAcceleration = LA; };
	inline void SetAngularVeloctiy(float AV) { AngularVelocity= AV; };
	inline void SetAngularAcceleration(float AA) { AngularAcceleration= AA; };

	virtual void Drive_Implementation(float deltaTime) override;
	virtual void Init_Implementation(AHBulletBase* initor) override;
	
};

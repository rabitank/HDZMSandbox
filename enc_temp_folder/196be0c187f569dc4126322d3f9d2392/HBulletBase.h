// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HBulletBase.generated.h"

UCLASS()
class HDZMSANDBOX_API AHBulletBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHBulletBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class USphereComponent* ComSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UProjectileMovementComponent* ComMovement;
	
protected:
	//UPROPERTY(EditDefaultsOnly, Category = "Bullet")
	//	class UCurveFloat* ContainEnergyCurve;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bullet")
	float RecoilForce;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bullet")
	float InitialSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bullet | Energy")
	float CurrentContainEnergy;
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bullet")
	float FlyStartedTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bullet")
	float LifeDuration;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bullet")
	FTimerHandle TimeHandle_FadeAway;
	
	//if true ,FadeAway really fadeaway after life ending ,false means use Exploed in FadeAway
	//@DefaultValue: false
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet")
	bool bCanFadeAway;

	UFUNCTION()
	void OnActorHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& hit);

	UFUNCTION(BlueprintNativeEvent)
	void FadeAway();
	
	UFUNCTION(BlueprintNativeEvent)
	void Exploed();

	UFUNCTION(BlueprintNativeEvent)
	void ApplyRecoilForce();

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	//	class UParticleSystemComponent* ComEffectParticle;
	//
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	//	class UParticleSystem* ExplodeParticle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

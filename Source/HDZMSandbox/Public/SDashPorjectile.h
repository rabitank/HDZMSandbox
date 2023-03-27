// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SDashPorjectile.generated.h"

UCLASS()
class HDZMSANDBOX_API ASDashPorjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASDashPorjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USphereComponent* ComSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UProjectileMovementComponent* ComMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UParticleSystemComponent* ComEffectParticle;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UParticleSystem* ExploseParticle;

	UFUNCTION()
	void DashInstigatorAndDestory();


private:

	virtual void PostInitializeComponents() override;

	FTimerHandle TimerHandle_Flytime;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) ;

};

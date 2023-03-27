// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SBlackhole.generated.h"

UCLASS()
class HDZMSANDBOX_API ASBlackhole : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASBlackhole();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USphereComponent* ComSphere;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USphereComponent* ComBlackholeSphere;



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UProjectileMovementComponent* ComMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UParticleSystemComponent* ComEffectParticle;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	class URadialForceComponent* ComExplodForce;

private:
	UFUNCTION()
	virtual void OnActorOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

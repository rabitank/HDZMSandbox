// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectileBase.generated.h"

UCLASS()
class HDZMSANDBOX_API ASProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category = "Components")
		class USphereComponent* ComSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UProjectileMovementComponent* ComMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UParticleSystemComponent* ComEffectParticle;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "Effects")
	class UParticleSystem* ExplodeParticle;


	//must mark it with UFUNCTION if it be binded to Event
	UFUNCTION()
	void OnActorHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,FVector NormalImpulse,const FHitResult& hit);
	


	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void Explode();



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

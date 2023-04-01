// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectileBase.h"
#include "SMagicProjectile.generated.h"
class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;


UCLASS()
class HDZMSANDBOX_API ASMagicProjectile : public ASProjectileBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASMagicProjectile();



protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UAudioComponent* ComAudio;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USoundBase* ImpactAudio;
	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DamageAmount;

	
	//UPROPERTY(EditDefaultsOnly)
	//class UAudio

	//must mark it with UFUNCTION if it be binded to Event
	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);



	virtual void Explode_Implementation() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

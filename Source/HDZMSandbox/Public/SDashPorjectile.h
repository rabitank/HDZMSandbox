// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectileBase.h"
#include "SDashPorjectile.generated.h"
UCLASS()
class HDZMSANDBOX_API ASDashPorjectile : public ASProjectileBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASDashPorjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Explode_Implementation() override;
	void DashInstigator();
	
	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
		float TeleportDelay;
	
	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
		float DetonateDelay;


	FTimerHandle TimerHandle_Flytime;

private:

	virtual void PostInitializeComponents() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};

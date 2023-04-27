// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HBulletBase.h"
#include "HPrimaryBullet.generated.h"

UCLASS()
class HDZMSANDBOX_API AHPrimaryBullet : public AHBulletBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHPrimaryBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnActorOverlap(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SExplodingBarre.generated.h"

UCLASS()
class HDZMSANDBOX_API ASExplodingBarre : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ASExplodingBarre();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	//value type var should be adjusted by BP;
	//behavior bool type var should be writed in cpp;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* ComBarreMesh;
	
	UPROPERTY(VisibleAnywhere)
	class URadialForceComponent* ComExplodForce;

	//InitializeComponents post
	virtual void PostInitializeComponents() override;


private:
	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable)
	void FireImpulseForce();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGamePlayInterface.h"
#include "SPowerUpActor.generated.h"

UCLASS()
class HDZMSANDBOX_API ASPowerUpActor : public AActor, public ISGamePlayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
/// <summary>
/// @diescribtion: 如果有机会再说把,反正我健康药水已经写好了 T_T -> 没事 已经改了
/// </summary>
	ASPowerUpActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;


	FTimerHandle TImerHandle_RespawnHandle;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class USphereComponent* ComSphere;
	
	UPROPERTY(EditAnywhere, Category = "CoolDown")
		float CoolDownTime;
	
	void SetPowerupState(bool Pstate);
	void HiddenAndCooldownPowerup();
	
	UFUNCTION(NetMulticast,Reliable)
	void MulticastHiddenAndCooldownPowerup();

	UFUNCTION()
	void ShowPowerup();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

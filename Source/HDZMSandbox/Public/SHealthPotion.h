// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGamePlayInterface.h"
#include "SPowerUpActor.h"
#include "SHealthPotion.generated.h"

UCLASS()
class HDZMSANDBOX_API ASHealthPotion : public ASPowerUpActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASHealthPotion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* ComMesh;


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 CostCredits;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AddHealth;


};

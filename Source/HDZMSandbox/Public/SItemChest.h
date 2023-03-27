// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGamePlayInterface.h"
#include "SItemChest.generated.h"

UCLASS()
class HDZMSANDBOX_API ASItemChest : public AActor,public ISGamePlayInterface
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	float TargetPitch;

	void Interact_Implementation(APawn* InstigatorPawn);
public:	
	// Sets default values for this actor's properties
	ASItemChest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class	UStaticMeshComponent* ComBaseMesh;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	class	UStaticMeshComponent* ComLidMesh;
	//Lid: Gaizi 盖子	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

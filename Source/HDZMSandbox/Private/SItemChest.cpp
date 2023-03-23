// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemChest.h"

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	ComLidMesh->SetRelativeRotation(FRotator(TargetPitch, 0.f, 0.f));

}

// Sets default values
ASItemChest::ASItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ComBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("SItCComBaseMesh");
	RootComponent = ComBaseMesh;
	ComLidMesh = CreateDefaultSubobject<UStaticMeshComponent>("SItCComLidMesh");
	ComLidMesh->SetupAttachment(ComBaseMesh);

	TargetPitch = 110.f;
}

// Called when the game starts or when spawned
void ASItemChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


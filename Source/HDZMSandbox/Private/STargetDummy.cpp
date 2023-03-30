// Fill out your copyright notice in the Description page of Project Settings.



#include "STargetDummy.h"
#include <SAttributeComponent.h>
#include "Components/StaticMeshComponent.h"

// Sets default values
ASTargetDummy::ASTargetDummy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ComMesh = CreateDefaultSubobject<UStaticMeshComponent>("STarComStaticMesh");
	RootComponent = ComMesh;

	ComAttributes = CreateDefaultSubobject<USAttributeComponent>("STarComAttributes");
	ComAttributes->OnHealthChanged.AddDynamic(this,&ASTargetDummy::OnHealthChanged);
}

// Called when the game starts or when spawned
void ASTargetDummy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASTargetDummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASTargetDummy::OnHealthChanged(USAttributeComponent* owningComp, AActor* instigatorActor, float newHealth, float delta)
{
	if (delta < 0.f)
	{
		ComMesh->SetScalarParameterValueOnMaterials("HitTime",GetWorld()->GetTimeSeconds());
	}

}


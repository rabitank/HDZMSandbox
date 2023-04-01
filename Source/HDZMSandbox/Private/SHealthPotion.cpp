// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"
#include <SAttributeComponent.h>

// Sets default values
ASHealthPotion::ASHealthPotion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ComMesh = CreateDefaultSubobject<UStaticMeshComponent>("SHeaComMesh");
	RootComponent = ComMesh;

}

// Called when the game starts or when spawned
void ASHealthPotion::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	USAttributeComponent* comUSA =  Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
	if (comUSA)
	{
		if (comUSA->IsMaxHealth())
		{
			return;
		}

		comUSA->ApplyHealthChangeDelta(20.f);
		SetActorHiddenInGame(true);
		ComMesh->SetActive(false);
		GetWorldTimerManager().SetTimer(TimerHandle_ReInteractable, [this]() {SetActorHiddenInGame(false); ComMesh->SetActive(true); }, 10.f, false);
	}


}

// Called every frame
void ASHealthPotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"
#include <SAttributeComponent.h>
#include "SPlayerState.h"

// Sets default values
ASHealthPotion::ASHealthPotion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ComMesh = CreateDefaultSubobject<UStaticMeshComponent>("SHeaComMesh");
	RootComponent = ComMesh;

	CostCredits = 5.f;
	
	AddHealth = 20.f;


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
		ASPlayerState* state = Cast<ASPlayerState>(InstigatorPawn->GetPlayerState());
		if (ensure(state))
		{
			if (state->RemoveCredit(CostCredits) && comUSA->ApplyHealthChangeDelta(AddHealth, this))
			{
				HiddenAndCooldownPowerup();
			}
		}
		return;
	}

}




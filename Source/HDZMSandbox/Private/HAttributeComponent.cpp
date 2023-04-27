// Fill out your copyright notice in the Description page of Project Settings.


#include "HAttributeComponent.h"
#include "HGameModeBase.h"
#include "../HDZMSandbox.h"
#include "DrawDebugHelpers.h"

static TAutoConsoleVariable<float> CvarIncreaseMultiplier(TEXT("hd.DamageMultiplier"), 1.f, TEXT("Global IncreaseEnergy modifier for Attribute	Component."), ECVF_Cheat);

// Sets default values for this component's properties
UHAttributeComponent::UHAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	EnergyMax = 100;
	EnergyDangerou = 0.8 * EnergyMax;
	Energy = EnergyDangerou;
}


// Called when the game starts
void UHAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

bool UHAttributeComponent::ApplyEnergyChangeDelta(AActor* Instigator, float delta)
{
	//for God()
	if (!GetOwner()->CanBeDamaged() && delta < 0.f)
	{
		return false;
	}

	if (delta > 0.f)
	{
		float IncreaseMultiplier = CvarIncreaseMultiplier.GetValueOnGameThread();
		delta *= IncreaseMultiplier;
	}

	float oldEnergy = Energy;
	float NewEnergy = FMath::Clamp(Energy + delta, -1.f, EnergyMax+1);

	bool overMax = false;
	float actualDelta=0;

	//expend energe
	if (NewEnergy<0.f && delta <0.f)
	{
		//delta <0.f maybe not neccessry
		return false;
	}
	//over die
	else if(NewEnergy > EnergyMax)
	{
		// if oldEnergy is eual EnergyMax,actualDelta will be 1.f
		// should we broadcast died EnergyChanged?
		overMax = true;
		Energy = EnergyMax; 
	}
	//normal
	else 
	{
		Energy = NewEnergy; 
	}

	actualDelta = NewEnergy - oldEnergy;
	
	DrawDebugString(GetWorld(), GetOwner()->GetActorLocation(),FString::Printf(TEXT("delta Energy %f, curent Energy: %f"), actualDelta, Energy),NULL,FColor::Red,2.f);

	if (actualDelta != 0.f)
	{
		OnEnergyChanged.Broadcast(this, Instigator, Energy, actualDelta);
	};

	if (actualDelta > 0.f && Energy>EnergyDangerou)
	{
		LogOnScreen(this, "Energe has Reached to DangerousLevel", FColor::Yellow, 1.f);
		if (overMax)
		{
			AHGameModeBase* gameMode = GetWorld()->GetAuthGameMode<AHGameModeBase>();
			if (gameMode)
			{
				gameMode->OnActorKilled(GetOwner(), Instigator);
			}
		}
	};

	return actualDelta != 0;

}

// Called every frame
void UHAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHAttributeComponent::RestoreEnergyToInitLevel()
{
	if (IsDangerouOrInit())
		return;
	ApplyEnergyChangeDelta(GetOwner(), EnergyDangerou - Energy);
}


UHAttributeComponent* UHAttributeComponent::GetAttribute(AActor* actor)
{
	if (ensure(actor))
	{
		UHAttributeComponent* attribute = Cast<UHAttributeComponent>(actor->GetComponentByClass(UHAttributeComponent::StaticClass()));
		if (attribute)
			return attribute;
	
	}
	return nullptr;
}

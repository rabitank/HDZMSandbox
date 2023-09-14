// Fill out your copyright notice in the Description page of Project Settings.


#include "EmitteSystem/HEnergyComponent.h"
#include "DrawDebugHelpers.h"
#include <../HDZMSandbox.h>

static TAutoConsoleVariable<float> CvarIncreaseMultiplier(TEXT("hd.EnergyMultiplier"), 1.f, TEXT("Global IncreaseEnergy modifier for Energy Component."), ECVF_Cheat);
UHEnergyComponent::UHEnergyComponent()
{
	PrimaryComponentTick.bCanEverTick = true;


	EnergyMax = 100;
	EnergyDangerou = 0.8 * EnergyMax;
	Energy = EnergyDangerou;

	// ...
}


// Called when the game starts
void UHEnergyComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHEnergyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UHEnergyComponent::ApplyEnergyChangeDelta(AActor* Instigator, float delta)
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
	float NewEnergy = FMath::Clamp(Energy + delta, -1.f, EnergyMax);

	bool overMax = false;
	float actualDelta = 0;

	//expend energe
	if (NewEnergy < 0.f && delta < 0.f)
	{
		//delta <0.f maybe not neccessry
		return false;
	}
// 	over die
// 	else if (NewEnergy > EnergyMax)
// 	{
// 		// if oldEnergy is eual EnergyMax,actualDelta will be 1.f
// 		// should we broadcast died EnergyChanged?
// 		overMax = true;
// 		Energy = EnergyMax;
// 	}
	//normal
	else
	{
		Energy = NewEnergy;
	}

	actualDelta = NewEnergy - oldEnergy;

	DrawDebugString(GetWorld(), GetOwner()->GetActorLocation(), FString::Printf(TEXT("delta Energy %f, curent Energy: %f"), actualDelta, Energy), NULL, FColor::Red, 2.f);

	if (actualDelta != 0.f)
	{
		OnEnergyChanged.Broadcast(this, Instigator, Energy, actualDelta);
	};

	if (actualDelta > 0.f && Energy <= EnergyDangerou)
	{
		LogOnScreen(this, "Energe has Reached to DangerousLevel", FColor::Yellow, 1.f);
// 		if (overMax)
// 		{
// 			AHGameModeBase* gameMode = GetWorld()->GetAuthGameMode<AHGameModeBase>();
// 			if (gameMode)
// 			{
// 				gameMode->OnActorKilled(GetOwner(), Instigator);
// 			}
// 		}
	};

	return actualDelta != 0;

}

void UHEnergyComponent::RestoreEnergyToInitLevel()
{
	if (IsDangerouOrInit())
		return;
	ApplyEnergyChangeDelta(GetOwner(), EnergyDangerou - Energy);

}


UHEnergyComponent* UHEnergyComponent::GetAttribute(AActor* formActor)
{
	if (ensure(formActor))
	{

		UHEnergyComponent* comp= Cast<UHEnergyComponent>(formActor->GetComponentByClass(UHEnergyComponent::StaticClass()));
		if (comp)
		{
			return comp;
		}
	}

	return nullptr;
}

void UHEnergyComponent::PostInitProperties()
{

	Super::PostInitProperties();
	EnergyDangerou = EnergyMax * 0.8f;


}

#if WITH_EDITOR
void UHEnergyComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	EnergyDangerou = EnergyMax * 0.8f;

	Super::PostEditChangeProperty(PropertyChangedEvent);

}
#endif

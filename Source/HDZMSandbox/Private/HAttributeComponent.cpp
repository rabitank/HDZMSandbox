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

}

// Called when the game starts
void UHAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

bool UHAttributeComponent::ApplyHealthChangeDelta(AActor* Instigator, float delta)
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

	float oldHealth = Health;
	Health = FMath::Clamp(Health + delta, 0.f, HealthMax);

	float actualDelta = 0;
	actualDelta = Health - oldHealth;

	DrawDebugString(GetWorld(), GetOwner()->GetActorLocation(), FString::Printf(TEXT("delta Health %f, curent Health: %f"), actualDelta, Health), NULL, FColor::Red, 2.f);

	if (actualDelta != 0.f)
	{
		OnHealthChanged.Broadcast(this, Instigator, Health, actualDelta);
	};

	if (actualDelta < 0.f && Health < HealthDangerou)
	{
		LogOnScreen(this, "Health has Reached to DangerousLevel", FColor::Yellow, 1.f);
		if (!Health)
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

void UHAttributeComponent::RestoreHealthToFull()
{
	if (IsHealthFull())
		return;
	ApplyHealthChangeDelta(GetOwner(), HealthMax - Health);
}

void UHAttributeComponent::PostInitProperties()
{

	Super::PostInitProperties();
	HealthDangerou = HealthMax * HealthDangerouPersent;


}

#if WITH_EDITOR
void UHAttributeComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	HealthDangerou = HealthMax * HealthDangerouPersent;

	Super::PostEditChangeProperty(PropertyChangedEvent);

}
#endif

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



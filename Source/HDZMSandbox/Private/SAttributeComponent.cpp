// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"
#include "DrawDebugHelpers.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{

	HealthMax = 100;
	Health = HealthMax;
	// ...
}



bool USAttributeComponent::ApplyHealthChangeDelta(float delta)
{
	Health = FMath::Clamp(Health+delta, 0.f, HealthMax);
	DrawDebugString(GetWorld(), GetOwner()->GetActorLocation(),FString::Printf(TEXT("delta health %f, curent healty: %f"), delta, Health),NULL,FColor::Red,2.f);
	
	//broadcast what. where the dispatchers' eventInformations from.
	//make inputs is same with signature
	OnHealthChanged.Broadcast(this,nullptr,Health,delta);
	//.AddDyanmic is actually add delegate dispatcher
	return true;

}

bool USAttributeComponent::IsAlive()
{
	return Health > 0.f ;
}


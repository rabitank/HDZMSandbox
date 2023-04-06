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



USAttributeComponent* USAttributeComponent::GetAttributes(AActor* formActor)
{
	if(formActor)
		return  Cast<USAttributeComponent>(formActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	//safe
	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* queryActor)
{
	USAttributeComponent* attribute= GetAttributes(queryActor);
	if (attribute)
	{
		return attribute->IsAlive();
	}

	//@MayNeedFix: how to explain it is invalid?
	return false;
}

bool USAttributeComponent::ApplyHealthChangeDelta(float delta, AActor* InstigatorActor)
{
	float oldHealth = Health;
	Health = FMath::Clamp(Health+delta, 0.f, HealthMax);
	float actualDelta = Health - oldHealth;
	DrawDebugString(GetWorld(), GetOwner()->GetActorLocation(),FString::Printf(TEXT("delta health %f, curent healty: %f"), actualDelta, Health),NULL,FColor::Red,2.f);
	
	//broadcast what. where the dispatchers' eventInformations from.
	//make inputs is same with signature
	OnHealthChanged.Broadcast(this,InstigatorActor,Health,actualDelta);
	//.AddDyanmic is actually add delegate dispatcher
	return actualDelta !=0;

}

float USAttributeComponent::GetMaxHealth()
{
	return HealthMax;
}

bool USAttributeComponent::IsAlive()
{
	return Health > 0.f ;
}

bool USAttributeComponent::IsLowHealth()
{
	//@fixme: what is low level health?
	return Health / HealthMax < 0.5f;
}


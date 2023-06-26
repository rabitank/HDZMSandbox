// Fill out your copyright notice in the Description page of Project Settings.

#include "HGameModeFunctionLibrary.h"
#include "HAttributeComponent.h"

bool UHGameModeFunctionLibrary::ApplyEnergy(AActor* CauserActor, AActor* TargetActor, float DamageAmount)
{
	UHAttributeComponent* targetAttribute = UHAttributeComponent::GetAttribute(TargetActor);
	if(targetAttribute)
	{
		return targetAttribute->ApplyEnergyChangeDelta(CauserActor, DamageAmount);
	}
	return false;
}


bool UHGameModeFunctionLibrary::ApplyDirectionalEnergy(AActor* CauserActor, AActor* TargetActor, float DamageAmount, const FHitResult& hit)
{
	if (ApplyEnergy(CauserActor, TargetActor, DamageAmount))
	{
		UPrimitiveComponent* comHit = hit.GetComponent();
		if (comHit && comHit->IsSimulatingPhysics(hit.BoneName))
		{
			comHit->AddImpulseAtLocation(-hit.ImpactNormal * 30000.f, hit.ImpactPoint, hit.BoneName);
		}
		return true;
	}
	return false;


}

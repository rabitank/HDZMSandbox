// Fill out your copyright notice in the Description page of Project Settings.

#include "HGameModeFunctionLibrary.h"
#include "HAttributeComponent.h"



bool UHGameModeFunctionLibrary::ApplyDamage(AActor* CauserActor, AActor* TargetActor, float DamageAmount)
{
	if (!ensureMsgf(DamageAmount < 0.f, TEXT("ApplyDamged need negative value!")))
	{
		return false;
	}

	UHAttributeComponent* attribute = UHAttributeComponent::GetAttribute(TargetActor);
	if (attribute)
	{
		return attribute->ApplyHealthChangeDelta(CauserActor,DamageAmount);
	}
	return false;
}

bool UHGameModeFunctionLibrary::ApplyDirectionalDamage(AActor* CauserActor, AActor* TargetActor, float DamageAmount, const FHitResult& hit)
{

	if (ApplyDamage(CauserActor, TargetActor, DamageAmount))
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
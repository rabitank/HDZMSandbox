// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeFunctionLibrary.h"
#include "SAttributeComponent.h"

bool USGameModeFunctionLibrary::ApplyDamage(AActor* CauserActor, AActor* TargetActor, float DamageAmount)
{
	if (!ensureMsgf(DamageAmount < 0.f,TEXT("ApplyDamged need negative value!")))
	{
		return false;
	}

	USAttributeComponent* attribute = USAttributeComponent::GetAttributes(TargetActor);
	if (attribute)
	{
		return attribute->ApplyHealthChangeDelta(DamageAmount,CauserActor);
	}
	return false;
}

bool USGameModeFunctionLibrary::ApplyDirectionalDamage(AActor* CauserActor, AActor* TargetActor, float DamageAmount, const FHitResult& hit)
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

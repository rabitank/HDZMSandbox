// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionEffect_Thorns.h"
#include <SAttributeComponent.h>
#include <SActionComponent.h>
#include <SGameModeFunctionLibrary.h>

USActionEffect_Thorns::USActionEffect_Thorns()
{
	ThornsPercent = 0.2f;

	//means Inf
	Duration = 0.f;

	//means what?
	Period = 0.f;

}

void USActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	USAttributeComponent* attributeComp = USAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (attributeComp)
	{
		attributeComp->OnHealthChanged.AddDynamic(this,&USActionEffect_Thorns::OnHealthChanged);
	}
}

void USActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	USAttributeComponent* attributeComp = USAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (attributeComp)
	{
		attributeComp->OnHealthChanged.Remove(this, "OnHealthChanged");
			
	}
}

void USActionEffect_Thorns::OnHealthChanged(USAttributeComponent* owningComp, AActor* instigatorActor, float newHealth, float delta)
{
	AActor* onwer = GetOwningComponent()->GetOwner();
	if (delta < 0.f && onwer != instigatorActor)
	{
		int32 returnDamage = FMath::RoundToInt(delta * ThornsPercent);
		if (!returnDamage)
			return;

		if (instigatorActor)
		{
			USGameModeFunctionLibrary::ApplyDamage(onwer,instigatorActor, returnDamage);
		}
	}

}

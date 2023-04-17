// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerUp_Action.h"
#include <SCharacter.h>
#include "SActionComponent.h"

void ASPowerUp_Action::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn && ContainActionClass))
	{
		return;
	}

	USActionComponent* actions = USActionComponent::GetActions(InstigatorPawn);
	if (actions)
	{
		//avoid readd
		if (actions->GetAction(ContainActionClass))
		{
			FString mesg =FString::Printf(TEXT("Character Has had Action %s"),*GetNameSafe(ContainActionClass));
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, mesg);
		}
		//only call in Server,so don't check
		MultiCastAddAction(actions);
		HiddenAndCooldownPowerup();
	}

}

void ASPowerUp_Action::MultiCastAddAction_Implementation(USActionComponent* actions)
{ 
	actions->AddAction(this, ContainActionClass);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerUp_Credits.h"
#include "SPlayerState.h"

ASPowerUp_Credits::ASPowerUp_Credits()
{
	CreditsAmount = 80.f;
}

void ASPowerUp_Credits::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	ASPlayerState* ps = Cast<ASPlayerState> (InstigatorPawn->GetPlayerState());
	if (ps)
	{
		ps->AddCredit(CreditsAmount);
		HiddenAndCooldownPowerup();
	}

}

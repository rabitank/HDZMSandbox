// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "../HDZMSandbox.h"

ASPlayerState::ASPlayerState()
{
	Credit = 0;
	bReplicates = true;
}

void ASPlayerState::MulticastOnCreditsChanged_Implementation(ASPlayerState* ps, int32 newCredit, int32 delta)
{
	LogOnScreen(ps, "Is changing Credtis ,and should Brodcast", FColor::Green);
	OnCreditsChanged.Broadcast(this, newCredit, delta);
}

void ASPlayerState::AddCredit(int32 delta)
{
	if (delta < 0)
	{
		return;
	}
	Credit += delta;
	if (HasAuthority())
	{
		MulticastOnCreditsChanged(this, Credit, delta);
	}

}

bool ASPlayerState::RemoveCredit(int32 delta)
{
	if (delta<0||  Credit < delta )
	{
		return false ;
	}

	Credit -= delta;
	if(HasAuthority())
		MulticastOnCreditsChanged(this, Credit, -delta);
	return true;
}

int32 ASPlayerState::GetCredit() const
{
	return Credit;
}

void ASPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPlayerState, Credit);

}

// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "../HDZMSandbox.h"
#include "SSaveGame.h"

ASPlayerState::ASPlayerState()
{
	Credit = 0;
	bReplicates = true;
}

void ASPlayerState::OnRep_OnCreditsChanged(int32 oldCredit)
{
	LogOnScreen(this, "Is changing Credtis ,and should Brodcast", FColor::Green);
	OnCreditsChanged.Broadcast(this, Credit, Credit -  oldCredit);
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
		OnCreditsChanged.Broadcast(this, Credit, delta);
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
		OnCreditsChanged.Broadcast(this, Credit, delta);
	return true;
}

int32 ASPlayerState::GetCredit() const
{
	return Credit;
}

void ASPlayerState::SavePlayerState(class USSaveGame* currentSave)
{
	if(currentSave)
	currentSave->Credit = Credit;
}

void ASPlayerState::LoadPlayerState(class USSaveGame* currentSave)
{
	if (currentSave)
	{
		//Credit = currentSave->Credit;
		AddCredit(currentSave->Credit);
	};

}

void ASPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPlayerState, Credit);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "HGameModeBase.h"
#include "../HDZMSandbox.h"


void AHGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	LogOnScreen(this, "An Actor been killed!");
}

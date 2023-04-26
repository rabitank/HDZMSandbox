// Fill out your copyright notice in the Description page of Project Settings.


#include "HEmitAction.h"
#include "HEmitterComponent.h"
#include "../HDZMSandbox.h"

void UHEmitAction::Initialize(UHEmitterComponent* NewEmitterComp)
{
	EmitterCompnent = NewEmitterComp;
}

UHEmitAction::UHEmitAction()
{
	AcitonName = "Shoot";
	Duration = 1.f;
}

void UHEmitAction::StartShoot_Implementation(AActor* Instigator)
{
	UHEmitterComponent* ComOwner = GetOwningComponent();
	ComOwner->bIsShooting =true;

	EmitActionData.bIsRunning= true;
	EmitActionData.Instigator = Instigator;

	//@fix: the Event will be of EmitterCOmpoent
	//ComOwner->OnActionStarted.Broadcast(ComOwner, this);

	//only get time in server
	StartedTime = GetWorld()->GetTimeSeconds();

	LogOnScreen(GetOwningComponent(), "EmitAction Started", FColor::Yellow);

	
}

void UHEmitAction::StopShoot_Implementation(AActor* Instigator)
{
	EmitActionData.bIsRunning = false;
	UHEmitterComponent* comp = GetOwningComponent();
	if (ensure(comp))
	{
		comp->bIsShooting = false;
	}
}

bool UHEmitAction::CanStart_Implementation(AActor* Instigator)
{
	//avoid reStart without stop 
	if (IsShooting())
		return false;

	UHEmitterComponent* comp = GetOwningComponent();

	return !comp->bIsShooting;
}

class UHEmitterComponent* UHEmitAction::GetOwningComponent() const
{
	return EmitterCompnent;
}

class UWorld* UHEmitAction::GetWorld() const
{
	//outer is set when creating action via NewObject<T>
	AActor* actor = Cast<AActor>(GetOuter());
	if (actor)
	{
		return actor->GetWorld();
	}
	return nullptr;
}

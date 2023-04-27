// Fill out your copyright notice in the Description page of Project Settings.


#include "HEmitAction.h"
#include "HEmitterComponent.h"
#include "../HDZMSandbox.h"
#include "HActionComponent.h"

void UHEmitAction::Initialize(class UHActionComponent* InitActionComp)
{
	Super::Initialize(InitActionComp);

	UHEmitterComponent* NewEmitterComp = Cast<UHEmitterComponent>(InitActionComp->GetOwner()->GetComponentByClass(UHEmitterComponent::StaticClass()));
	if (!ensure(NewEmitterComp))
	{
		FString ActionMsg = FString::Printf(TEXT("owner:%s didnt has HEmitterComponent"), *GetNameSafe(GetOuter()));
		LogOnScreen(this, ActionMsg, FColor::Red, 0.f);
		EmitterCompnent = nullptr;

		return;
	}
	EmitterCompnent = NewEmitterComp;
}


UHEmitAction::UHEmitAction()
{

}

void UHEmitAction::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	EmitterCompnent->OnTrigerPressed(Instigator);
	EmitterCompnent->SetTriggerState( true);
	//@fix: the Event will be of EmitterCOmpoent
	//ComOwner->OnActionStarted.Broadcast(ComOwner, this);

}

void UHEmitAction::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	UHEmitterComponent* comp = EmitterCompnent;
	if (ensure(comp))
	{
		comp->SetTriggerState(false);
	}
}


bool UHEmitAction::CanStart_Implementation(AActor* Instigator)
{
	
	//contain IsTrigering, The Action Tag Triger is in sync with Emitter IsTrigger
	bool IsActionOK = Super::CanStart_Implementation(Instigator);

	if (!IsActionOK)
	{
		return false;
	}

	//if Can't Emit, no anyReaction
	if (EmitterCompnent->GetCurrentCore()->CanEmit(Instigator) )
	{
		return true;
	}

	return false;

}


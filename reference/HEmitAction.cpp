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

	if (ensure(EmitterCompnent))
	{
		EmitterCompnent->OnTrigerPressed(Instigator);
		EmitterCompnent->SetTriggerState(true);
	}
}

void UHEmitAction::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	if (ensure(EmitterCompnent))
	{
		EmitterCompnent->OnTrigerReleased(Instigator);
		EmitterCompnent->SetTriggerState(false);
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

	return true;

}


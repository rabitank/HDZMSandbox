// Fill out your copyright notice in the Description page of Project Settings.


#include "HAction.h"
#include "HActionComponent.h"

void UHAction::StartAction_Implementation(AActor* Instigator)
{

	UHActionComponent* ComOwner = GetOwningComponent();
	
	ComOwner->ActiveGameplayTags.AppendTags(GrantTags);

	ActionData.bIsRunning = true;
	ActionData.Instigator = Instigator;

	ComOwner->OnActionStarted.Broadcast(ComOwner, this);

	StartedTime = GetWorld()->GetTimeSeconds();

}

void UHAction::StopAction_Implementation(AActor* Instigator)
{
	UHActionComponent* ComOwner = GetOwningComponent();

	ComOwner->ActiveGameplayTags.RemoveTags(GrantTags);

	ActionData.bIsRunning = false;
	ComOwner->OnActionStoped.Broadcast(ComOwner, this);

}

void UHAction::Initialize(UHActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
}

bool UHAction::IsRunning()
{
	return ActionData.bIsRunning;

}

UHAction::UHAction()
{
	bIsAutoStart = false;
}

class UWorld* UHAction::GetWorld() const
{
	AActor* actor = Cast<AActor>(GetOuter());
	if (actor)
	{
		return actor->GetWorld();
	}
	return nullptr;
}

bool UHAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning())
		return false;

	UHActionComponent* comp = GetOwningComponent();
	if (comp->ActiveGameplayTags.HasAny(BlockTags))
	{
		return false;
	}

	return true;

}



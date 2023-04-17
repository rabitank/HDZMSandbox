// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"
#include "Logging/LogMacros.h"
#include "SActionComponent.h"
#include "../HDZMSandbox.h"
#include "Net/UnrealNetwork.h"


void USAction::Initialize(USActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
}


void USAction::StartAction_Implementation(AActor* Instigator)
{
	//GetNameSafe(UO* ) will get class FName number safely

	//LogOnScreen(GetOuter(), FString::Printf(TEXT("Started Action: %s"), *GetNameSafe(this)),FColor::Red);



	USActionComponent* ComOwner = GetOwningComponent();
	ComOwner->ActiveGameplayTags.AppendTags(GrantTags);

	RepActionData.bIsRunning = true;
	RepActionData.Instigator = Instigator;
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	//LogOnScreen(GetOuter(), FString::Printf(TEXT("Stop Action: %s"), *GetNameSafe(this)),FColor::White);


	//@Fixme:
	//Client: first bIsRuning changed to false then call stopAction,should not check.
	// maybe server should
	//ensureAlways(bIsRunning);

	USActionComponent* ComOwner = GetOwningComponent();
	ComOwner->ActiveGameplayTags.RemoveTags(GrantTags);

	RepActionData.bIsRunning = false;
	RepActionData.Instigator = Instigator;

}


bool USAction::CanStart_Implementation(AActor* Instigator)
{
	//avoid reStart without stop 
	if (IsRunning())
		return false;

	USActionComponent* comp = GetOwningComponent();
	if (comp->ActiveGameplayTags.HasAny(BlockTags))
	{
		return false;
	}
	return true;
}




bool USAction::IsRunning()
{
	return RepActionData.bIsRunning;
}


//@figureOut: why do this?
void USAction::OnRep_ActionData()
{
	if (RepActionData.bIsRunning)
	{
		StartAction(RepActionData.Instigator);
	}
	if (!RepActionData.bIsRunning)
	{
		StopAction(RepActionData.Instigator);
	}


}
USAction::USAction()
{
	bIsAutoStart = false;
}

class USActionComponent* USAction::GetOwningComponent() const
{
// 	AActor* outer = Cast<AActor>(GetOuter());
// 	if (outer)
// 	{
// 		return  Cast<USActionComponent>(outer->GetComponentByClass(USActionComponent::StaticClass()));
// 
// 	}

	return ActionComp;

}

class UWorld* USAction::GetWorld() const
{
	//outer is set when creating action via NewObject<T>
	AActor* actor = Cast<AActor>(GetOuter());
	if (actor)
	{
		return actor->GetWorld();
	}
	return nullptr;
}



void USAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAction, RepActionData);
	DOREPLIFETIME(USAction, ActionComp);

}
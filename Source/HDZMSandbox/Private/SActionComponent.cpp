// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionComponent.h"
#include "SAction.h"
#include "../HDZMSandbox.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"

//is it important?
void USActionComponent::AddAction(AActor* Instigator,TSubclassOf<USAction> ActionClass)
{

	if (!ensure(ActionClass))
	{
		return;
	}

	if (!Instigator->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Client is trying addAction,but forbidened"));
		return;
	}

	//NewObject<classNAme> creaete an TsubClass Instance return className*
	//outer : like owner? would be used to call more high level method in Action;
 	USAction* actionIns =  NewObject<USAction>(GetOwner(),ActionClass);
	if (ensure(actionIns))
	{
		actionIns->Initialize(this);
		Actions.Add(actionIns);

		if (actionIns->bIsAutoStart && ensure(actionIns->CanStart(Instigator)))
		{
			actionIns->StartAction(Instigator);
		}
	}
}

USAction* USActionComponent::GetAction(TSubclassOf<USAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return nullptr;
	}
	
	for (USAction* action : Actions)
	{
		if (action && action->IsA(ActionClass))
		{
			return action;
		}
	}
	return nullptr;


}

bool USActionComponent::StartActionByName(AActor* Instigator, FName actionName)
{

	for (USAction* action : Actions)
	{
		if (action->AcitonName == actionName)
		{
			if (!action->CanStart(Instigator))
			{
				FString  Msg = FString::Printf(TEXT("Failed to run: %s"), *actionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Msg);



				continue;
			}

			//Is Client?
			//Actor in  Client Hasnot Authority.
			//@fixme: HasAuthority() seems not always call correctly
			if (!GetOwner()->HasAuthority())
			{
				ServerStartActionByName(Instigator, actionName);
			}

			action->StartAction(Instigator);
			return true;
		}
	}

	return false;
}

void USActionComponent::ServerStartActionByName_Implementation(AActor* Instigator, FName actionName)
{
	StartActionByName(Instigator, actionName);
}

void USActionComponent::ServerStopActionByName_Implementation(AActor* Instigator, FName actionName)
{
	StopActionByName(Instigator, actionName);
}


bool USActionComponent::StopActionByName(AActor* Instigator, FName actionName)
{
	for (USAction* action : Actions)
	{
		if (action->AcitonName == actionName)
		{
			if (action->IsRunning())
			{
				action->StopAction(Instigator);
				
				if (!GetOwner()->HasAuthority())
				{
					ServerStopActionByName(Instigator, actionName);
				}

				return true;
			}

		}
	}

	return false;
}

void USActionComponent::RemoveAction(USAction* actionToRemove)
{
	if (ensure(actionToRemove && !actionToRemove->IsRunning()))
	{
		Actions.Remove(actionToRemove);
	}
}

USActionComponent* USActionComponent::GetActions(AActor* fromactor)
{
	if (fromactor)
		return  Cast<USActionComponent>(fromactor->GetComponentByClass(USActionComponent::StaticClass()));
	//safe
	return nullptr;
}

USActionComponent::USActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

}


void USActionComponent::BeginPlay()
{
	Super::BeginPlay();

	//initAction in Server
	if (GetOwner()->HasAuthority())
	{
		for (TSubclassOf<USAction> action : DefaultActions)
		{
			AddAction(GetOwner(),action); //defalut USAction SubAction(bp subInstanceClass)
		}
	}
}


// Called every frame
void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::White, DebugMsg);

	for (USAction* action: Actions)
	{
		FColor TextColor = action->IsRunning() ? FColor::Green: FColor::White;
		FString ActionMsg = FString::Printf(TEXT("[%s] Action:%s"),*GetNameSafe(GetOwner()),*action->AcitonName.ToString() );
		LogOnScreen(this, ActionMsg,TextColor, 0.f);
	}

}

void USActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(USActionComponent, Actions);

}

bool USActionComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool wroteSth = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (auto action : Actions)
	{
		if (action)
		{
			wroteSth |= Channel->ReplicateSubobject(action, *Bunch,*RepFlags);
		}
	}
	return wroteSth;

}


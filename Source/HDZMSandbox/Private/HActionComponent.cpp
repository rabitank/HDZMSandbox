// Fill out your copyright notice in the Description page of Project Settings.


#include "HActionComponent.h"
#include "HAction.h"
#include "../HDZMSandbox.h"

// Sets default values for this component's properties
UHActionComponent::UHActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UHActionComponent::AddAction(AActor* Instigator, TSubclassOf<UHAction> ActionClass)
{
	if (!ensure(ActionClass))
		return;

	UHAction* actionIns =  NewObject<UHAction>(GetOwner(),ActionClass);
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

class UHAction* UHActionComponent::GetAction(TSubclassOf<UHAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return nullptr;
	}

	for (UHAction* action : Actions)
	{
		if (action && action->IsA(ActionClass))
		{
			return action;
		}
	}

	return nullptr;
}

bool UHActionComponent::StartActionByName(AActor* Instigator, FName actionName)
{
	for (UHAction* action : Actions)
	{
		if (action->AcitonName == actionName)
		{
			if (!action->CanStart(Instigator))
			{
				FString  Msg = FString::Printf(TEXT("Failed to start: %s"), *actionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Msg);

				continue;
			}

			action->StartAction(Instigator);
			return true;
		}
	}

	return false;

}

bool UHActionComponent::StopActionByName(AActor* Instigator, FName actionName)
{
	for (UHAction* action : Actions)
	{
		if (action->AcitonName == actionName)
		{
			if (action->IsRunning())
			{
				action->StopAction(Instigator);

				return true;
			}
		}
	}

	return false;

}

void UHActionComponent::RemoveAction(UHAction* actionToRemove)
{
	if (ensure(actionToRemove && !actionToRemove->IsRunning()))
	{
		Actions.Remove(actionToRemove);
	}

}

UHActionComponent* UHActionComponent::GetActions(AActor* fromactor)
{
	if (fromactor)
		return  Cast<UHActionComponent>(fromactor->GetComponentByClass(UHActionComponent::StaticClass()));
	//safe
	return nullptr;
}

// Called when the game starts
void UHActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::White, DebugMsg);

	for (UHAction* action : Actions)
	{
		FColor TextColor = action->IsRunning() ? FColor::Green : FColor::White;
		FString ActionMsg = FString::Printf(TEXT("[%s] Action:%s"), *GetNameSafe(GetOwner()), *action->AcitonName.ToString());
		LogOnScreen(this, ActionMsg, TextColor, 0.f);
	}
}


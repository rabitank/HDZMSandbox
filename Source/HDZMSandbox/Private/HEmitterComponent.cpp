// Fill out your copyright notice in the Description page of Project Settings.


#include "HEmitterComponent.h"
#include "HEmitInterface.h"
#include "HEmitterCoreBase.h"
#include "../HDZMSandbox.h"
#include "HAttributeComponent.h"
#include "HPlayerCharacter.h"
#include "Engine/StaticMeshSocket.h"
#include "Components/StaticMeshComponent.h"

// Sets default values for this component's properties
UHEmitterComponent::UHEmitterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...

	bIsTriggering = false;

	DefaultMuzzelSocketName = TEXT("Muzzle");
	CoreSlotsNum = 1;

	CurrentCoreIndex = 0;
}

// Called when the game starts
void UHEmitterComponent::BeginPlay()
{
	Super::BeginPlay();

	//init Core
	for (TSubclassOf<UHEmitterCoreBase> EmitCoreClass : DefaultEmitterCoreClass)
	{
		AddCore(GetOwner(), EmitCoreClass); //defalut USAction SubAction(bp subInstanceClass)
	}
	CurrentCoreIndex = EmitterCores.Num()-1;

	// ...
	
}


UHEmitterCoreBase* UHEmitterComponent::SlideCore(float val)
{
	if ( bIsTriggering && CurrentCore->IsShooting())
	{
		return nullptr;
	}
	if (EmitterCores.Num()<2)
	{
		return nullptr;
	}
	float OldIndex = CurrentCoreIndex;
	CurrentCoreIndex += val;
	if (floor(CurrentCoreIndex) != floor(OldIndex))
	{
		if (floor(CurrentCoreIndex) >= EmitterCores.Num())
		{
			CurrentCoreIndex = 0;
			CurrentCore = *EmitterCores.begin();
		}
		else if (floor(CurrentCoreIndex) < 0)
		{
			CurrentCoreIndex = EmitterCores.Num() - 1;
			CurrentCore = EmitterCores.Last();
		}
		else
		{
			CurrentCore = EmitterCores[floor(CurrentCoreIndex)];
		}

		return CurrentCore;
	}

	return nullptr;

}

UHEmitterCoreBase* UHEmitterComponent::SelectedCoreByNum(int val)
{
	if (bIsTriggering && CurrentCore->IsShooting() )
	{
		return nullptr;
	}
	if (val == floor(CurrentCoreIndex))
		return nullptr;

	if (!EmitterCores.IsValidIndex(val))
		return nullptr;

	CurrentCoreIndex = val;
	CurrentCore = EmitterCores[val];
	return CurrentCore;
}

FTransform UHEmitterComponent::GetDefaultMuzzleTransform()
{

	AHPlayerCharacter* player = Cast<AHPlayerCharacter>(GetOwner());
	if (player)
	{
// 		USceneComponent* EmitterMove = player->GetEmitterMoveComp();
// 		if (ensure(EmitterMove))
// 		{
// 			return player->GetEmitterMoveComp()->GetComponentTransform();
// 		}
	}
	return FTransform(GetOwner()->GetActorRotation(), GetOwner()->GetActorLocation() + 80*GetOwner()->GetActorForwardVector());

}

void UHEmitterComponent::AddCore(AActor* Instigator, TSubclassOf<UHEmitterCoreBase> CoreClass)
{
	if (!CoreClass)
		return;

	if (EmitterCores.Num() >= CoreSlotsNum)
	{
		LogOnScreen(this, "AddCore filed! no enough Slot");
		return;
	}
	UHEmitterCoreBase* CoreIns = NewObject<UHEmitterCoreBase>(GetOwner(), CoreClass);
	if (ensure(CoreIns))
	{
		CoreIns->Initialize(this);
		EmitterCores.Add(CoreIns);
		CurrentCore = CoreIns;
	}
}

class UHEmitterCoreBase* UHEmitterComponent::GetCore(TSubclassOf<UHEmitterCoreBase> CoreClass)
{
	if (!ensure(CoreClass))
	{
		return nullptr;
	}

	for (UHEmitterCoreBase* CoreIns: EmitterCores)
	{
		if (CoreIns && CoreIns->IsA(CoreClass))
		{
			return CoreIns;
		}
	}
	return nullptr;

}

void UHEmitterComponent::RemoveCore(UHEmitterCoreBase* CoreToRemove)
{
	int32 key  = EmitterCores.Find(CoreToRemove);
	if (key != -1)
	{
		EmitterCores.RemoveAt(key);
	}
}

void UHEmitterComponent::OnTrigerPressed(AActor* instigator)
{
	//has checked Canemit() in EmitAction;
	if (ensure(CurrentCore))
	{
		CurrentCore->OnPressedTrigger(instigator);

		//IHEmitInterface::Execute_Emit(CurrentCore, instigator);
	}
	else
	{
		FString msg = FString::Printf(TEXT("%s: Emitter Trigger Preseaed filed: Current core is Nullptr"), *GetNameSafe(instigator));
		LogOnScreen(this, msg, FColor::Yellow);
	}

}

void UHEmitterComponent::OnTrigerReleased(AActor* instigator)
{
	//has checked Canemit() in EmitAction;
	if (ensure(CurrentCore))
	{
		CurrentCore->OnReleasedTriger(instigator);
	}
	else
	{
		FString msg = FString::Printf(TEXT("%s: Emitter Trigger Released filed: Current core is Nullptr"), *GetNameSafe(instigator));
		LogOnScreen(this, msg, FColor::Yellow);
	}

}

// Called every frame
void UHEmitterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::White, DebugMsg);

	FColor currentCoreColor = bIsTriggering ? FColor::Yellow : FColor::Green;
	if (CurrentCore)
	{
		currentCoreColor = CurrentCore->IsShooting() ? FColor::Red : currentCoreColor;

		for (UHEmitterCoreBase* core : EmitterCores)
		{
			FColor TextColor = core == CurrentCore ? currentCoreColor : FColor::Blue;
			FString CoreMsg = FString::Printf(TEXT("[%s] CurrentCore:%s"), *GetNameSafe(GetOwner()), *CurrentCore->CoreName.ToString());
			LogOnScreen(this, CoreMsg, TextColor, 0.f);
		}

	}
		
}



UHEmitterComponent* UHEmitterComponent::GetEmitter(AActor* actor)
{
	return Cast<UHEmitterComponent>(actor->GetComponentByClass(UHEmitterComponent::StaticClass()));
}
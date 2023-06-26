// Fill out your copyright notice in the Description page of Project Settings.


#include "HEmitterCoreBase.h"
#include "HEmitterComponent.h"
#include "HPlayerCharacter.h"
#include "HAttributeComponent.h"
#include "../HDZMSandbox.h"
#include "HBulletBase.h"

UHEmitterCoreBase::UHEmitterCoreBase()
{
	ComOwnerEnergySource = nullptr;
	//default: Spawn bullet when EmitAction over;

	CoolDownDuration = 10.f;
	LastShootTime = -CoolDownDuration;
	bIsAbsorbable = true;

	EnergyDemand = 20.f;
	BackInitDuration = 0.1f;

	//@FixMe:
	//rewrite in BP
	//Duration = SpawnDuration + BackInitDuration;
}




void UHEmitterCoreBase::Initialize(UHEmitterComponent* EmitterComp)
{
	if (ensure(EmitterComp))
	{
		ComOwnerEmitter = EmitterComp;

		UHAttributeComponent* energySource = UHAttributeComponent::GetAttribute(EmitterComp->GetOwner());

		if (ensure(energySource))
			ComOwnerEnergySource = energySource;
		else
			ComOwnerEnergySource = nullptr;

		return;
	}
	ComOwnerEmitter = nullptr;

}

void UHEmitterCoreBase::Emit_Implementation(AActor* Instigator)
{
	StartShoot(Instigator);
}


bool UHEmitterCoreBase::CanEmit(AActor* Instigator)
{
	APawn* OwnerPawn =Cast<APawn>(ComOwnerEmitter->GetOwner());
	if (ensure(OwnerPawn))
	{
		AHPlayerCharacter* HPlayer = Cast<AHPlayerCharacter>(OwnerPawn);
		
		//Player is Emiiter
		if (HPlayer)
		{
			//need cooldown
			if (GetWorld()->GetTimeSeconds() - LastShootTime < CoolDownDuration)
			{
				return false;
			};

			//need affored energy
			UHAttributeComponent* attribute = UHAttributeComponent::GetAttribute(OwnerPawn);
			if (!attribute->IsAffordable(EnergyDemand))
			{
				return false;
			}

		}
		//non Player Pawn don't need check cooldown and energy 
		return true ;
	}
	//non Pawn don't need check; . set by other loop
	return true;
}

void UHEmitterCoreBase::OnReleasedTriger_Implementation(AActor* Instiagtor)
{

}

bool UHEmitterCoreBase::OnPressedTrigger_Implementation(AActor* Instigator)
{
	if (!CanEmit(Instigator))
	{
		LogOnScreen(this, "CurrrentCore can't emit!");
		return false;
	}

	return true;
}

void UHEmitterCoreBase::StopShoot_Implementation(AActor* Instigator)
{

	LastShootTime = GetWorld()->GetTimeSeconds();
	bIsShooting = false;

	FString msg = FString::Printf(TEXT("Core: %s StopShooting"), *CoreName.ToString());
	LogOnScreen(this, msg, FColor::Yellow);

	ComOwnerEmitter->OnShootStoped.Broadcast(this,Instigator);
	
}

void UHEmitterCoreBase::StartShoot_Implementation(AActor* Instigator)
{
	bIsShooting = true;

	FString msg = FString::Printf(TEXT("Core: %s StartShooting"), *CoreName.ToString());
	LogOnScreen(this, msg, FColor::Green);

	ComOwnerEmitter->OnShootStarted.Broadcast(this,Instigator);




}


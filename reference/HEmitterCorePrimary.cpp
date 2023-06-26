// Fill out your copyright notice in the Description page of Project Settings.


#include "HEmitterCorePrimary.h"
#include "../HDZMSandbox.h"
#include "HEmitterComponent.h"
#include "HAttributeComponent.h"
#include "GameFramework/Actor.h"

UHEmitterCorePrimary::UHEmitterCorePrimary()
{
	//you need to keep the trigger to spawn bullet
	TriggerTimeDuration = 0.05f;
	CoreName = TEXT("PrimaryCore");


}

bool UHEmitterCorePrimary::OnPressedTrigger_Implementation(AActor* Instigator)
{
	if (!Super::OnPressedTrigger_Implementation(Instigator))
	{
		return false;
	};

	FTimerDelegate dele;
	dele.BindUFunction(this, "StartShoot", Instigator);
	GetWorld()->GetTimerManager().SetTimer(TimeHandle_StartShoot, dele, TriggerTimeDuration, false);

	return true;

}

void UHEmitterCorePrimary::OnReleasedTriger_Implementation(AActor* Instiagtor)
{
	Super::OnReleasedTriger_Implementation(Instiagtor);
	StopShoot(Instiagtor);

}

void UHEmitterCorePrimary::StopShoot_Implementation(AActor* Instigator)
{
	Super::StopShoot_Implementation(Instigator);
	
	if (TimeHandle_StartShoot.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(TimeHandle_StartShoot);
	}
}

void UHEmitterCorePrimary::StartShoot_Implementation(AActor* Instiagtor)
{
	Super::StartShoot_Implementation(Instiagtor);


	FActorSpawnParameters SMagspawnPars;
	SMagspawnPars.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


	APawn* pawnInstigator = Cast<APawn>(Instiagtor);
	//if nullptr , bullet will hurt everyone;
	SMagspawnPars.Instigator = pawnInstigator;
	
	//emit from Muzzle Slot Position!
	//HEmitterComp should keep the slot/slot name;

	AActor* spawnedBullet = nullptr;
	if (ensure(BulletClass))
		spawnedBullet = GetWorld()->SpawnActor<AActor>(BulletClass, ComOwnerEmitter->GetDefaultMuzzleTransform(), SMagspawnPars);


	//if successed spawned , ApplyEnergy
	if (spawnedBullet)
	{
		if (ComOwnerEnergySource)
		{
			ComOwnerEnergySource->ApplyEnergyChangeDelta(ComOwnerEnergySource->GetOwner(), -EnergyDemand);
		}

		FTimerDelegate dele;
		dele.BindUFunction(this, "StopShoot", Instiagtor);
		GetWorld()->GetTimerManager().SetTimer(TimeHandle_StopShoot, dele, 0.2f, false);
		return;
	}

	LogOnScreen(this, "Bullet spawn failed!");

}

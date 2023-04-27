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
	
	SpawnDuration = 0.2f;
	BackInitDuration = 0.1f;

	//@FixMe:
	//rewrite in BP
	//Duration = SpawnDuration + BackInitDuration;
}

//remove it to PrimaryCore?
void UHEmitterCoreBase::SpawnBulletDelay_Implementation(AActor* instigator)
{
	FActorSpawnParameters SMagspawnPars;
	SMagspawnPars.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	APawn* pawnInstigator = Cast<APawn>(instigator);
	if (pawnInstigator)
	{
		SMagspawnPars.Instigator = pawnInstigator;
		
	}
	//emit from Muzzle Slot Position!
	//HEmitterComp should keep the slot/slot name;
	
	//@FixedMe , also should 
	LogOnScreen(instigator, "Bullet spawn");

	AActor* spawnedBullet = nullptr;
	if (ensure(BulletClass))
		spawnedBullet =  GetWorld()->SpawnActor<AActor>(BulletClass, ComOwnerEmitter->GetDefaultMuzzleTransform(), SMagspawnPars);
	//if successed spawned , ApplyEnergy
	if (spawnedBullet)
	{
		if (ComOwnerEnergySource)
		{
			ComOwnerEnergySource->ApplyEnergyChangeDelta(instigator, -EnergyDemand);
		}
	}

	//@fixme: move it(the connection between stop start and spawn function) to its sub class
	FTimerDelegate dele;
	dele.BindUFunction(this,"StopShoot",instigator);
	GetWorld()->GetTimerManager().SetTimer(TimeHandle_PrepareOverStopShooting, dele,SpawnDuration, false);

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

void UHEmitterCoreBase::OnPressedTrigger_Implementation(AActor* Instigator)
{

}

void UHEmitterCoreBase::StopShoot_Implementation(AActor* Instigator)
{

	LastShootTime = GetWorld()->GetTimeSeconds();
	bIsShooting = false;

	FString msg = FString::Printf(TEXT("Core: %s StopShooting"), *CoreName.ToString());
	LogOnScreen(this, msg, FColor::Yellow);

	OnShootStoped.Broadcast(this,Instigator);
	
}

void UHEmitterCoreBase::StartShoot_Implementation(AActor* Instigator)
{
	bIsShooting = true;

	FString msg = FString::Printf(TEXT("Core: %s StartShooting"), *CoreName.ToString());
	LogOnScreen(this, msg, FColor::Green);

	OnShootStarted.Broadcast(this,Instigator);



	//@fixme: move it(the connection between stop start and spawn function) to its sub class
	FTimerDelegate dele;
	dele.BindUFunction(this, TEXT("SpawnBulletDelay"), Instigator);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_SpawnBulletDelay, dele, SpawnDuration, false);

}


// Fill out your copyright notice in the Description page of Project Settings.


#include "HEmitterCoreBase.h"
#include "HEmitterComponent.h"
#include "HPlayerCharacter.h"
#include "HAttributeComponent.h"
#include "../HDZMSandbox.h"
#include "HBulletBase.h"

UHEmitterCoreBase::UHEmitterCoreBase()
{
	ComEnergySource = nullptr;
	//default: Spawn bullet when EmitAction over;

	CoolDownDuration = 10.f;
	LastShootTime = -CoolDownDuration;
	bIsAbsorbable = true;

	EnergyDemand = 20.f;
	
	SpawnDuration = 0.2f;
	BackInitDuration = 0.1f;

	//@FixMe:
	//rewrite in BP
	Duration = SpawnDuration + BackInitDuration;
}

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
		spawnedBullet =  GetWorld()->SpawnActor<AActor>(BulletClass, GetOwningComponent()->GetDefaultMuzzleTransform(), SMagspawnPars);

	FTimerDelegate dele;
	dele.BindUFunction(this,"StopShoot",instigator);
	GetWorld()->GetTimerManager().SetTimer(TimeHandle_PrepareOverStopShooting, dele,SpawnDuration, false);


	//if successed spawned , ApplyEnergy
	if (spawnedBullet)
	{
		if (ComEnergySource)
		{
			ComEnergySource->ApplyEnergyChangeDelta(instigator, -EnergyDemand);
		}
	}
	
}


void UHEmitterCoreBase::Initialize(UHAttributeComponent* energySource, UHEmitterComponent* EmitterComp)
{
	if (ensure(energySource))
		ComEnergySource = energySource;
	else
		ComEnergySource = nullptr;
	Super::Initialize(EmitterComp);
}

void UHEmitterCoreBase::Emit_Implementation(AActor* Instigator)
{

	StartShoot(Instigator);

}


bool UHEmitterCoreBase::CanEmit(AActor* Instigator)
{
	APawn* OwnerPawn =Cast<APawn>(GetOwningComponent()->GetOwner());
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
		return Super::CanStart(Instigator);
	}



	//non Pawn don't need check; . set by other loop
	return true;
	
	
}

void UHEmitterCoreBase::StopShoot_Implementation(AActor* Instigator)
{
	Super::StopShoot_Implementation(Instigator);
	LastShootTime = GetWorld()->GetTimeSeconds();
	
	FString msg = FString::Printf(TEXT("EmitterCore %s:EmitAction Over"), *CoreName.ToString());
	LogOnScreen(this, msg, FColor::Yellow);

}

void UHEmitterCoreBase::StartShoot_Implementation(AActor* Instigator)
{
	Super::StartShoot_Implementation(Instigator);

	FTimerDelegate dele;
	dele.BindUFunction(this, TEXT("SpawnBulletDelay"), Instigator);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_SpawnBulletDelay, dele, SpawnDuration, false);



}


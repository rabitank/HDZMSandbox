// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"
#include "DrawDebugHelpers.h"
#include "Math/UnrealMathUtility.h"
#include "SGameModeBase.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<float> CvarDamageMultiplier(TEXT("su.DamageMultiplier"),1.f, TEXT("Global Damage modifier for Attribute	Component."), ECVF_Cheat);


// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	HealthMax = 100;
	Health = HealthMax;
	
	RageMax = 100;
	Rage= RageMax;

	SetIsReplicatedByDefault(true);

	// ...
}

void USAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAttributeComponent, Health);
	DOREPLIFETIME(USAttributeComponent, HealthMax);
	
	//DOREPLIFETIME_CONDITION(USAttributeComponent, HealthMax,COND_InitialOnly);
	//COND_InitialOnly: only send once when initial
	//COND_OwnerOnly: Getonwer()? , if has a banch same class actors in client, only send to the onwer Instance?
	DOREPLIFETIME(USAttributeComponent, Rage);
	DOREPLIFETIME(USAttributeComponent, RageMax);
	//DOREPLIFETIME_CONDITION(USAttributeComponent, RageMax, COND_OwnerOnly);

}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* formActor)
{
	if(formActor)
		return  Cast<USAttributeComponent>(formActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	//safe
	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* queryActor)
{
	USAttributeComponent* attribute= GetAttributes(queryActor);
	if (attribute)
	{
		return attribute->IsAlive();
	}

	//@MayNeedFix: how to explain it is invalid?
	return false;
}


bool USAttributeComponent::ApplyHealthChangeDelta(float delta, AActor* InstigatorActor)
{
	//for God()
	if (!GetOwner()->CanBeDamaged() && delta<0.f)
	{
		return false;
	}
	
	if (delta < 0.f)
	{
		float DamageMultiplier = CvarDamageMultiplier.GetValueOnGameThread();
		delta *= DamageMultiplier;
	}

	float oldHealth = Health;
	float NewHealth = FMath::Clamp(Health+delta, 0.f, HealthMax);

	float actualDelta = NewHealth - oldHealth;
	//do culculate on Server
	if (GetOwner()->HasAuthority())
	{
		Health = NewHealth;
		//DrawDebugString(GetWorld(), GetOwner()->GetActorLocation(),FString::Printf(TEXT("delta health %f, curent healty: %f"), actualDelta, Health),NULL,FColor::Red,2.f);

		if (actualDelta != 0.f)
		{
			MultiCastOnHealthChanged(this, InstigatorActor, Health, actualDelta);
		}

		if (actualDelta < 0.f && Health <= 0.f)
		{
			//GameMode is on serveice , only one and live with whole game;
			ASGameModeBase* gameMode = GetWorld()->GetAuthGameMode<ASGameModeBase>();
			if (gameMode)
			{
				gameMode->OnActorKilled(GetOwner(), InstigatorActor);
			}
		}

	}
	return actualDelta !=0;

}
void USAttributeComponent::MultiCastOnHealthChanged_Implementation(USAttributeComponent* owncomp, AActor* Instigator, float newHealth, float actualDelta)
{
	//broadcast what. where the dispatchers' eventInformations from.
	//make inputs is same with signature
	OnHealthChanged.Broadcast(owncomp, Instigator, newHealth, actualDelta);
	//.AddDyanmic is actually add delegate dispatcher
}

void USAttributeComponent::MultiCastOnRageChanged_Implementation(USAttributeComponent* owncomp, AActor* Instigator, float newRage, float actualDelta)
{
	OnRageChanged.Broadcast(owncomp, Instigator, newRage, actualDelta);
}

bool USAttributeComponent::ApplyRageChangeDelta(float delta, AActor* Instigator /*= nullptr*/)
{
	float oldRage = Rage;
	Rage = FMath::Clamp(Rage + delta, 0.f, RageMax);
	float actualDelta = Rage - oldRage;
	//DrawDebugString(GetWorld(), GetOwner()->GetActorLocation(),FString::Printf(TEXT("delta Rage %f, curent healty: %f"), actualDelta, Rage),NULL,FColor::Red,2.f);
	if (actualDelta != 0.f)
	{
		MultiCastOnRageChanged(this, Instigator, Rage, actualDelta);
		return true;
	}

	return false;

}

bool USAttributeComponent::Kill(AActor* theInstigator)
{
	return ApplyHealthChangeDelta(-HealthMax, theInstigator);
}

float USAttributeComponent::GetMaxHealth()
{
	return HealthMax;
}


bool USAttributeComponent::IsAlive()
{
	return Health > 0.f ;
}

bool USAttributeComponent::IsLowHealth()
{
	//@fixme: what is low level health?
	return Health / HealthMax < 0.5f;
}


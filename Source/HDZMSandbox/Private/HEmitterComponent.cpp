// Fill out your copyright notice in the Description page of Project Settings.


#include "HEmitterComponent.h"
#include "HEmitInterface.h"
#include "../HDZMSandbox.h"
#include "HAttributeComponent.h"
#include "HPlayerCharacter.h"
#include "Engine/StaticMeshSocket.h"
#include "Components/StaticMeshComponent.h"
#include <EmitteSystem/HEmitter.h>
#include "EmitteSystem/HEmitterPattern.h"

// Sets default values for this component's properties
UHEmitterComponent::UHEmitterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}

// Called when the game starts
void UHEmitterComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerEmitter = Cast<AHEmitter>(GetOwner());
	LogOnScreen(this, "HEmitterCompoenntInit", FColor::Red);
	//init 
	SpawnAndInitPatternWithDelay(BKEpClass, BKEmittePattern);
	SpawnAndInitPatternWithDelay(FWEpClass, FWEmittePattern);

	FWEmittePattern->PatternActive();
	BKEmittePattern->PatternRelax();
	CurrentPattern = FWEmittePattern;


}


bool UHEmitterComponent::SpawnAndInitPatternWithDelay(TSubclassOf<AHEmitterPattern> pattern,AHEmitterPattern*& patternIns, float delay/*=0.2f*/)
{

	if (pattern)
	{
		FActorSpawnParameters PatternParams;
		PatternParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		//@Discribtion: Instigator will pass to sender by getInstigator /¼´Apawn ->Player
		PatternParams.Instigator = GetOwner()->GetInstigator();
		PatternParams.Owner = GetOwner();

		patternIns = GetWorld()->SpawnActor<AHEmitterPattern>(pattern, OwnerEmitter->GetTransform(), PatternParams);
		if (ensure(patternIns))
		{

			//patternIns->AttachToComponent(OwnerEmitter->GetMesh(), { EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false }, TEXT("EmittePattern"));
			patternIns->AttachToActor(OwnerEmitter, FAttachmentTransformRules::SnapToTargetIncludingScale);

			FTimerHandle TimerHandle_InitEmitterPattern;
			FTimerDelegate delegate;
			delegate.BindUFunction(this, "DelayInitPattern_Elaps", patternIns);

			GetOwner()->GetWorldTimerManager().SetTimer(TimerHandle_InitEmitterPattern, delegate, delay, false);

		};

		return true;
	}
	UE_LOG(LogTemp,Warning,TEXT("EmptyPattern can't be Init!"));
	return false;
}

void UHEmitterComponent::DelayInitPattern_Elaps(AHEmitterPattern* EpIns)
{
	EpIns->InitPattern();
	bAbleShoot = true;
}


void UHEmitterComponent::SwitchPattern()
{

	check(BKEmittePattern);
	check(FWEmittePattern);

	BKEmittePattern->PatternRelax();
	FWEmittePattern->PatternRelax();
	
	auto newPattern = BKEmittePattern;
	BKEmittePattern = FWEmittePattern;
	FWEmittePattern = newPattern;
	
	if (OwnerEmitter->IsBackward())
	{
		CurrentPattern = BKEmittePattern;
		BKEmittePattern->PatternActive();

	}
}

void UHEmitterComponent::OnSwitchAimingState()
{
	CurrentPattern->PatternRelax();
	if (CurrentPattern == FWEmittePattern)
	{
		BKEmittePattern->PatternActive();
		CurrentPattern = BKEmittePattern;
	}
	else
	{
		FWEmittePattern->PatternActive();
		CurrentPattern = FWEmittePattern;
	}
}

bool UHEmitterComponent::ChangeFWEp(TSubclassOf<AHEmitterPattern> newPattern, AActor* instigator)
{

	return true;
}

bool UHEmitterComponent::Shoot(AActor* instegator)
{



	return true;
}



void UHEmitterComponent::RemoveCore(UHEmitterCoreBase* CoreToRemove)
{

}

void UHEmitterComponent::OnTrigerPressed(AActor* instigator)
{
	if (OwnerEmitter->IsBackward())
	{
		ensure(BKEmittePattern->Shoot(instigator) && CurrentPattern==BKEmittePattern);
	}
	else ensure(FWEmittePattern->Shoot(instigator) && CurrentPattern== FWEmittePattern);

}

void UHEmitterComponent::OnTrigerReleased(AActor* instigator)
{
	CurrentPattern->StopShoot(instigator);
	//has checked Canemit() in EmitAction;
// 	if (ensure(CurrentCore))
// 	{
// 		CurrentCore->OnReleasedTriger(instigator);
// 	}
// 	else
// 	{
// 		FString msg = FString::Printf(TEXT("%s: Emitter Trigger Released filed: Current core is Nullptr"), *GetNameSafe(instigator));
// 		LogOnScreen(this, msg, FColor::Yellow);
// 	}

}



//Called every frame
void UHEmitterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::White, DebugMsg);

	FString active(TEXT("Active"));
	FString relaxe(TEXT("Relaxe"));

	FColor  curColor = BKEmittePattern == CurrentPattern ? FColor::White : FColor::Black;
	FString CoreMsg;

	if (BKEmittePattern->IsActivePattern())
	{
		CoreMsg = FString::Printf(TEXT("[%s]:%s "), TEXT("BKPattern"),*active);
	}
	else
	{
		CoreMsg = FString::Printf(TEXT("[%s]:%s "), TEXT("BKPattern"),*relaxe);
	}
			
	LogOnScreen(this, CoreMsg, curColor, 0.f);

	curColor = FWEmittePattern== CurrentPattern ? FColor::White : FColor::Black;

	if (FWEmittePattern->IsActivePattern())
	{
		CoreMsg = FString::Printf(TEXT("[%s]:%s "), TEXT("FWPattern"),*active);
		if (bShouldAim)
		{
			//TODO:a stable Distance connnection
			FWEmittePattern->SetSenderRotateTo(OwnerEmitter->GetAimingDistance());
		}
	}
	else
	{
		CoreMsg = FString::Printf(TEXT("[%s]:%s "), TEXT("FWPattern"),*relaxe);
	}
			
	LogOnScreen(this, CoreMsg, curColor, 0.f);



}



UHEmitterComponent* UHEmitterComponent::GetEmitter(AActor* actor)
{
	return Cast<UHEmitterComponent>(actor->GetComponentByClass(UHEmitterComponent::StaticClass()));
}



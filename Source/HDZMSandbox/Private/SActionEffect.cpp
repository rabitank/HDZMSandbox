// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionEffect.h"
#include "../Public/SActionComponent.h"


USActionEffect::USActionEffect()
{
	bIsAutoStart = true;
}

void USActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	if (Duration > 0.f)
	{
		FTimerDelegate dele;
		dele.BindUFunction(this,"StopAction",Instigator);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_StopAction, dele, Duration, false);

	}
	if (Period > 0.f)
	{
		FTimerDelegate dele;
		dele.BindUFunction(this,"ExcuteBufferEffect",Instigator);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_ExcuteBuffer, dele, Period,true);
	}
}

void USActionEffect::StopAction_Implementation(AActor* Instigator)
{
	//avoid a little bit delay course different behaviour
	if (GetWorld()->GetTimerManager().GetTimerRemaining(TimerHandle_ExcuteBuffer) < KINDA_SMALL_NUMBER )
	{
		ExcuteBufferEffect(Instigator);
	}
	Super::StopAction_Implementation(Instigator);

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_StopAction);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_ExcuteBuffer);

	// buffer is over , remove from tag and actions
	USActionComponent* comAction = GetOwningComponent();
	if (comAction)
	{
		comAction->RemoveAction(this);
	}

	//no reference to this action ,its memory would been restored by UE
}

void USActionEffect::ExcuteBufferEffect_Implementation(AActor* Instigator)
{

}


// Fill out your copyright notice in the Description page of Project Settings.


#include "EmitteSystem/HEmitterAnim.h"
#include "EmitteSystem/HEmitter.h"
#include "Kismet/KismetMathLibrary.h"



EHAimingInput UHEmitterAnim::CalculateAimingInputDirection(float Angle, float FLth /*= -30.f*/, float FRth/*=30.f*/, float FLLth /*= -95.f*/, float FRRth/*=95.f*/)
{
	if (Angle >= 0)
	{
		if(Angle<=FRth) return EHAimingInput::ForwardDirection;
		if (Angle <= FRRth) return EHAimingInput::RightDirection;
	}
	else
	{
		if (Angle >= FLth) return EHAimingInput::ForwardDirection;
		if (Angle >= FLLth) return EHAimingInput::LeftDirection;
	}

	return EHAimingInput::NegateDirection;

}

void UHEmitterAnim::UpdateState()
{
	ShootType = OwningEmitter->ShootType;
	bIsAim = OwningEmitter->bIsAim;
	bHasMovementInput = OwningEmitter->bHasMovementInput;
	MovementInputAce = OwningEmitter->InputAcceleration;
	ScaleFac = OwningEmitter->FinalScaleFac;

	AimingInputDirection = CalculateAimingInputDirection(
		UKismetMathLibrary::NormalizedDeltaRotator(UKismetMathLibrary::MakeRotFromX(MovementInputAce),
			OwningEmitter->CharacterRotator).Yaw
	);
}

void UHEmitterAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!IsValid(OwningEmitter)) return;
	UpdateState();
}

void UHEmitterAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	OwningEmitter = Cast<AHEmitter>(GetOwningActor());
	if(IsValid(OwningEmitter))
		ControledCharacter = OwningEmitter->OwnerPawn;

}

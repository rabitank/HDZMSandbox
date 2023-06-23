// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "State/HShootType.h"
#include "HEmitterAnim.generated.h"

/**
 * 
 */

UENUM(Blueprintable)
enum class EHAimingInput :uint8
{
	NegateDirection,
	ForwardDirection,
	RightDirection,
	LeftDirection
};


UCLASS(Blueprintable)
class HDZMSANDBOX_API UHEmitterAnim : public UAnimInstance
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Reference")
		class AHPlayerCharacter* ControledCharacter;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "Reference")
		class AHEmitter* OwningEmitter;


	// ue 手册中说的枚举前加E理解为类名而不是变量名.
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Used States")
		EHShootType  ShootType {EHShootType::None};
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Used States")
		EHAimingInput AimingInputDirection {EHAimingInput::ForwardDirection};

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Used States")
		bool  bIsAim{ false };
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Used States")
		bool  bHasMovementInput{ false };

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Used States")
		FVector  MovementInputAce {FVector::ZeroVector};
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Used States")
		float ScaleFac {1.f};
		
	friend class AHEmitter;

private:
	EHAimingInput CalculateAimingInputDirection(float Angle,float FLth = -30.f, float FRth=30.f, float FLLth = -95.f, float FRRth=95.f);


	void UpdateState();

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override; // 原生动画更新点. 自己去看源码.
	virtual void NativeInitializeAnimation() override;

};

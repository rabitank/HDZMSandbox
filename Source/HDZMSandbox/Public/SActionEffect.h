// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SActionEffect.generated.h"

/**
 * 
 */
UCLASS()
class HDZMSANDBOX_API USActionEffect : public USAction
{
	GENERATED_BODY()

protected:
	///
	///@Attention: pls just Dynamic add USActionEffect(or only by actionComp.AddAction())
	/// 



	UPROPERTY()
		FTimerHandle TimerHandle_StopAction;

	UPROPERTY()
		FTimerHandle TimerHandle_ExcuteBuffer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
		float Duration;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = "Effect")
		float Period;
	
	UFUNCTION(BlueprintNativeEvent,Category = "Effect")
	void ExcuteBufferEffect(AActor* Instigator);



public:

	UFUNCTION(BlueprintCallable)
		float GetTimeRemaining();

	USActionEffect();

	virtual	void StartAction_Implementation(AActor* Instigator) override;

	virtual	void StopAction_Implementation(AActor* Instigator) override;
	


};

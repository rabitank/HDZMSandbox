// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "HAction.h"
#include "HEmitAction.generated.h"

/**
 * 
 */

UCLASS(Blueprintable)
class HDZMSANDBOX_API UHEmitAction :public UHAction
{
	GENERATED_BODY()
protected:
	UPROPERTY()
		class UHEmitterComponent* EmitterCompnent;

public:
	virtual void Initialize(class UHActionComponent* InitActionComp) override;

	//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	//		class UTexture2D* ActionIcon;

	UHEmitAction();

	void StartAction_Implementation(AActor* Instigator);

	void StopAction_Implementation(AActor* Instigator);

	virtual bool CanStart_Implementation(AActor* Instigator) override;

};



// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SBTService_CheckIfLowHealth.generated.h"

/**
 * 
 */
UCLASS()
class HDZMSANDBOX_API USBTService_CheckIfLowHealth : public UBTService
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector bIfLowHealthKey; //avoid hard code name

	/** update next tick interval
	* this function should be considered as const (don't modify state of object) if node is not instanced! */
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	// more info about BT:https://docs.unrealengine.com/5.1/zh-CN/behavior-tree-in-unreal-engine---quick-start-guide/



};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SBTService_CheckAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class HDZMSANDBOX_API USBTService_CheckAttackRange : public UBTService
{
	GENERATED_BODY()
public:
	// UBTService is a BackGroundService Node class for BT , it will reflash BB on times ,doesn't effect task
	// can/should override : TickNode(), OnSearchStart()
 	//F12 to konw details
	
protected:

	UPROPERTY(EditAnywhere, Category = "AI")
		FBlackboardKeySelector AttackRangeKey; //avoid hard code name

	/** update next tick interval
	* this function should be considered as const (don't modify state of object) if node is not instanced! */
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	// more info about BT:https://docs.unrealengine.com/5.1/zh-CN/behavior-tree-in-unreal-engine---quick-start-guide/

};

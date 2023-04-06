// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/SBTTask_RestoreToMaxHealth.h"
#include <SAttributeComponent.h>
#include "GameFramework/Character.h"
#include "AIController.h"

EBTNodeResult::Type USBTTask_RestoreToMaxHealth::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	//it be consider as constant and be used as normal node; 
	//so should check hardly;

	AAIController* aiContoller = OwnerComp.GetAIOwner();
	if (ensure(aiContoller))
	{
		ACharacter* myPawn = Cast<ACharacter>(aiContoller->GetPawn());
		if (myPawn == nullptr)
		{
			return EBTNodeResult::Failed; //no instanced pawn,failed;
		}

		USAttributeComponent* pawnHealthCom = Cast<USAttributeComponent>(myPawn->GetComponentByClass(USAttributeComponent::StaticClass()));

		if (pawnHealthCom==nullptr)
		{
			return EBTNodeResult::Failed;
		}

		if (!pawnHealthCom->IsMaxHealth())
		{
			pawnHealthCom->ApplyHealthChangeDelta(pawnHealthCom->GetMaxHealth(),myPawn);
		}
		return EBTNodeResult::Succeeded;

	}
	return EBTNodeResult::Failed;

}

// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckIfLowHealth.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "SAttributeComponent.h"


void USBTService_CheckIfLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* blackBoard = OwnerComp.GetBlackboardComponent();
	if (ensure(blackBoard))
	{
		//make sure is Aipawn
		APawn* AIpawn = OwnerComp.GetAIOwner()->GetPawn();
		if (ensure(AIpawn))
		{
			USAttributeComponent* myAttributeCom = Cast<USAttributeComponent>(AIpawn->GetComponentByClass(USAttributeComponent::StaticClass()));

			bool bIsLowHealth = myAttributeCom->IsLowHealth();
			blackBoard->SetValueAsBool(bIfLowHealthKey.SelectedKeyName, bIsLowHealth);
		}
	}
}

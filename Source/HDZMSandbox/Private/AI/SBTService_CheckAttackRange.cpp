// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	UBlackboardComponent* blackBoard = OwnerComp.GetBlackboardComponent();
	if (ensure(blackBoard))
	{
		AActor* targetPawn = Cast<AActor>(blackBoard->GetValueAsObject("TargetActor"));
		if (targetPawn)
		{
			AAIController* ownerAIController = OwnerComp.GetAIOwner(); //make sure is ai controlling now?
			if (ensure(ownerAIController))
			{
				APawn* AIpawn = ownerAIController->GetPawn();
				if (ensure(AIpawn))
				{
					float distance = FVector::Distance(AIpawn->GetActorLocation(), targetPawn->GetActorLocation());
					
					bool bIswithinRange = distance<2000.f;

					bool bHasLOS = false;
					if (bIswithinRange)
					{
						bHasLOS = ownerAIController->LineOfSightTo(targetPawn);
					}

					blackBoard->SetValueAsBool(AttackRangeKey.SelectedKeyName,bIswithinRange&& bHasLOS);

				}



			}

		}

	}


}

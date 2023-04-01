// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void ASAIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BehaviorTree);

	auto pawnPlayer = UGameplayStatics::GetPlayerPawn(this, 0);
	if (pawnPlayer)
	{
		this->GetBlackboardComponent()->SetValueAsVector("MoveToLocation",pawnPlayer->GetActorLocation());
		GetBlackboardComponent()->SetValueAsObject("TargetActor", pawnPlayer);
	}


}

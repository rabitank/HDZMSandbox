// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"




void ASAIController::BeginPlay()
{
	Super::BeginPlay();

	if(ensureMsgf(BehaviorTree,TEXT("BahaviorTree is nullptr, pls assign BehaviorTree in bp")))
	RunBehaviorTree(BehaviorTree);

// 	auto pawnPlayer = UGameplayStatics::GetPlayerPawn(this, 0);
// 	if (pawnPlayer)
// 	{
// 		GetBlackboardComponent()->SetValueAsObject("TargetActor", pawnPlayer);
// 	}
// 

}



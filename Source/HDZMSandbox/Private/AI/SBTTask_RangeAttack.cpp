// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_RangeAttack.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type USBTTask_RangeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//it be consider as constant and be used as normal node; 
	//so should check hardly;

	AAIController* aiContoller = OwnerComp.GetAIOwner();
	if (ensure(aiContoller))
	{
		ACharacter* myPawn = Cast<ACharacter>(aiContoller->GetPawn());
		if (myPawn==nullptr)
		{
			return EBTNodeResult::Failed; //no instanced pawn,failed;
		}

		//Attack?
		FVector MuzzleLocation = myPawn->GetMesh()->GetSocketLocation("Muzzle_01");
			
		AActor* targetActor =Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		if (targetActor == nullptr)
		{
			return EBTNodeResult::Failed;
		}
		
		FVector direction = targetActor->GetActorLocation() - MuzzleLocation;
		FRotator MuzzleRotation = direction.Rotation();

		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		spawnParams.Instigator = myPawn;

		AActor* spawnActor = GetWorld()->SpawnActor<AActor>(ProjectileClass,MuzzleLocation,MuzzleRotation,spawnParams);
		
		return spawnActor ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;

	}
	return EBTNodeResult::Failed;

}

// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "SAttributeComponent.h"
#include "BrainComponent.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ComPawnSense = CreateDefaultSubobject<UPawnSensingComponent>("SAICComPawnSense");
	ComSAttribute = CreateDefaultSubobject<USAttributeComponent>("SAICComSAttribute");
	

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	HitTimeParametersName = TEXT("HitTime");
}


void ASAICharacter::OnHealthChanged_Implementation(USAttributeComponent* owningComp, AActor* instigatorActor, float newHealth, float delta)
{
	if (delta < 0.f)
	{
		//react to Damage for others
		if (instigatorActor != this)
		{
			//magicrojectile's Instigator
			SetTargetActor(instigatorActor);

			//flash
			GetMesh()->SetScalarParameterValueOnMaterials(HitTimeParametersName,GetWorld()->TimeSeconds);

		}

		if (newHealth <= 0.f)
		{


			//stop BT
			AAIController* aiContr =  Cast<AAIController>(GetController());
			if (aiContr)
			{
				//whats brainComp?: view https://zhuanlan.zhihu.com/p/512649286
				aiContr->GetBrainComponent()->StopLogic("character been killed ");

			}
			//ragdol
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

			//disapeare
			SetLifeSpan(10.f);
		}
	}
}

void ASAICharacter::SetTargetActor(AActor* target)
{
	AAIController* aIcontroller = Cast<AAIController>(GetController());

	if (aIcontroller)
	{
		UBlackboardComponent* comBlackBoard = aIcontroller->GetBlackboardComponent();
		comBlackBoard->SetValueAsObject("TargetActor", target);
		//DrawDebugString(GetWorld(), GetActorLocation(), "Find Target!", nullptr, FColor::Yellow, 2.f);
	}

}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ComPawnSense->OnSeePawn.AddDynamic(this, &ASAICharacter::OnSeePawn);
	ComSAttribute->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
}

//the Pawn : what he saw;
void ASAICharacter::OnSeePawn(APawn* Pawn)
{

	SetTargetActor(Pawn);
}


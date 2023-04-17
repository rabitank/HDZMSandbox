// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "SAttributeComponent.h"
#include "BrainComponent.h"
#include "SWorldUserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SActionComponent.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ComPawnSense = CreateDefaultSubobject<UPawnSensingComponent>("SAICComPawnSense");
	ComSAttribute = CreateDefaultSubobject<USAttributeComponent>("SAICComSAttribute");
	ComSAction = CreateDefaultSubobject<USActionComponent>("SAICComSAction");


	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	// now magicPorjecitle exploed is by code in its onoverlap();

	HitTimeParametersName = TEXT("HitTime");
	TargetActorKey = "TargetActor";
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

		if (ActiveHealthBar == nullptr)
		{
			//5 kinds UO is acceptable as owning
			//	UWidget 
			//	UWidgetTree 
			//	APlayerController
			//	UGameInstance 
			//	UWorld 
			ActiveHealthBar =  CreateWidget<USWorldUserWidget>(GetWorld() ,AIHealthWidgetClass);
			if (ActiveHealthBar)
			{
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}

		}

		//died
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

			//close old collision
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();

			
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
		comBlackBoard->SetValueAsObject(TargetActorKey, target);
		//DrawDebugString(GetWorld(), GetActorLocation(), "Find Target!", nullptr, FColor::Yellow, 2.f);
	}

}

AActor* ASAICharacter::GetTargetActor()
{
	AAIController* aIcontroller = Cast<AAIController>(GetController());

	if (aIcontroller)
	{
		UBlackboardComponent* comBlackBoard = aIcontroller->GetBlackboardComponent();
		return Cast<AActor>(comBlackBoard->GetValueAsObject(TargetActorKey));
	}
	return nullptr;
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ComPawnSense->OnSeePawn.AddDynamic(this, &ASAICharacter::OnSeePawn);
	ComSAttribute->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
}


void ASAICharacter::MultiCastCreateFindWidget_Implementation()
{
	USWorldUserWidget* findWidget = CreateWidget<USWorldUserWidget>(GetWorld(), AIFindPlayerWidgetClass);
	if (ensureMsgf(findWidget, TEXT("AIFindPlayerWidgetClass shoulb be subclass of SworldWidget")))
	{
		findWidget->AttachedActor = this;
		//10: z buffer value-> depth in render
		findWidget->AddToViewport(10);
	}
}


//the Pawn : what he saw;
void ASAICharacter::OnSeePawn(APawn* Pawn)
{
	if (GetTargetActor() != Pawn)
	{
		SetTargetActor(Pawn);

		//PawnSenseComponent don't replicate,OnSeePawn only run in server
		//so don't need Authority check
		MultiCastCreateFindWidget();
	}


}


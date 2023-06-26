// Fill out your copyright notice in the Description page of Project Settings.


#include "HPrimaryBullet.h"
#include "GameFramework/Actor.h"
#include "HPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SphereComponent.h"
#include "HGameModeFunctionLibrary.h"

// Sets default values
AHPrimaryBullet::AHPrimaryBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ComMesh->OnComponentBeginOverlap.AddDynamic(this,&AHPrimaryBullet::OnActorOverlap);

}

// Called when the game starts or when spawned
void AHPrimaryBullet::BeginPlay()
{
	Super::BeginPlay();
}


void AHPrimaryBullet::OnActorOverlap(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor && OtherActor != GetInstigator())
	{

		//USActionComponent* actionCom = Cast<USActionComponent>(OtherActor->GetComponentByClass(USActionComponent::StaticClass()));
		//if (actionCom && actionCom->ActiveGameplayTags.HasTag(ParryTag))
		//{
		//	ComMovement->Velocity = -ComMovement->Velocity;
		//	SetInstigator(Cast<APawn>(OtherActor));
		//	return;
		//}
		//if (USGameModeFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, DamageAmount, SweepResult))
		//{
		//	//super  method
		//	FadeAway();
		//	
		//	//actionCom->AddAction(GetInstigator(), BurningEffectClass);
		//	return;
		//};

		//@FixedMe: make DirectionImpluse reational with Bullet Ins;
		if (UHGameModeFunctionLibrary::ApplyDirectionalEnergy(GetInstigator(), OtherActor, 5.f, SweepResult))
		{
			FadeAway();
			return;
		}

	}
}

// Called every frame
void AHPrimaryBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Fill out your copyright notice in the Description page of Project Settings.


#include "SDashPorjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASDashPorjectile::ASDashPorjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DetonateDelay = 0.2f;
	TeleportDelay = 0.2f;

}

// Called when the game starts or when spawned
void ASDashPorjectile::BeginPlay()
{
	Super::BeginPlay();

	ComSphere->IgnoreActorWhenMoving(GetInstigator(),true);
	GetWorldTimerManager().SetTimer(TimerHandle_Flytime,this,&ASDashPorjectile::Explode,DetonateDelay);
	
}

//OnActorHit -> Explode -> Explode_Implementation
void ASDashPorjectile::Explode_Implementation()
{
	//if hit, dont need timer
	GetWorldTimerManager().ClearTimer(TimerHandle_Flytime);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplodeParticle,GetActorLocation(),GetActorRotation());
	//close moveEffect
	ComEffectParticle->DeactivateSystem();
	//ComMovement->SetVelocityInLocalSpace(FVector(0.f));
	ComMovement->StopMovementImmediately();
	SetActorEnableCollision(false);

	FTimerHandle TimerHandel_Teleport;
	GetWorldTimerManager().SetTimer(TimerHandel_Teleport,this,&ASDashPorjectile::DashInstigator,TeleportDelay);
}

void ASDashPorjectile::DashInstigator()
{
	AActor* instigator = GetInstigator();
	if (ensure(instigator))
	{
		//T_T: UE had supported TeleportTo()
		instigator->TeleportTo(this->GetActorLocation(),instigator->GetActorRotation(),false,false);
	}
	//dash
	Destroy();// or not
}


void ASDashPorjectile::PostInitializeComponents()
{
	//Call SuperMethod!!!!!!!!!!!!!
	Super::PostInitializeComponents();
}

// Called every frame
void ASDashPorjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}




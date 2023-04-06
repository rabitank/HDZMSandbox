// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerUpActor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"

// Sets default values
ASPowerUpActor::ASPowerUpActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ComSphere = CreateDefaultSubobject<USphereComponent>("SPowComSphere");
	RootComponent = ComSphere;
	ComSphere->SetSphereRadius(20.f);
	CoolDownTime = 10.f;

}

// Called when the game starts or when spawned
void ASPowerUpActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASPowerUpActor::Interact_Implementation(APawn* InstigatorPawn)
{

	//logic
}

void ASPowerUpActor::SetPowerupState(bool Pstate)
{
	SetActorEnableCollision(Pstate);
	RootComponent->SetVisibility(Pstate, true);
	//for visiable 

}

void ASPowerUpActor::ShowPowerup()
{
	SetPowerupState(true);
}

void ASPowerUpActor::HiddenAndCooldownPowerup()
{

	SetPowerupState(false);
	GetWorldTimerManager().SetTimer(TImerHandle_RespawnHandle,this,&ASPowerUpActor::ShowPowerup,CoolDownTime,false );

}

// Called every frame
void ASPowerUpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


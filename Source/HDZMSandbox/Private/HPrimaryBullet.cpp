// Fill out your copyright notice in the Description page of Project Settings.


#include "HPrimaryBullet.h"
#include "GameFramework/Actor.h"
#include "HPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AHPrimaryBullet::AHPrimaryBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHPrimaryBullet::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void AHPrimaryBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


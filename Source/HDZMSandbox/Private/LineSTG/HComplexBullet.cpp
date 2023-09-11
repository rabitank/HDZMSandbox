// Fill out your copyright notice in the Description page of Project Settings.


#include "LineSTG/HComplexBullet.h"
#include "LineSTG/HDriver.h"

#include "IMovieSceneTracksModule.h"


AHComplexBullet::AHComplexBullet()
{
	NativeDriverClass= UHDriver::StaticClass();
}

void AHComplexBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHComplexBullet::Drive_Implementation(float deltaTime)
{
	Super::Drive_Implementation(deltaTime);
	Driver->DriverUpdate(deltaTime);
}

void AHComplexBullet::Init_Implementation(AHBulletBase* initor)
{
	Super::Init_Implementation(initor);
	AHComplexBullet* initorCB = Cast<AHComplexBullet>(initor);
	if(ensure(initorCB))
	{
		Driver = initorCB->Driver->CloneToAttach(this);
	};
}

/// 记得 Call Parent!!!! 
void AHComplexBullet::BeginPlay()
{
	Super::BeginPlay();
	// init 的actor(也就是spawn中init的actor)已经cloneattach了Driver.
	if(!IsValid(Driver))
	{
		const auto newName = FString::Printf(TEXT("Native%s"),*NativeDriverClass->GetName());
		Driver = NewObject<UHDriver>(this,NativeDriverClass.Get(),FName(*newName));
		Driver->InitAttachment(this);
	}
	
}

AHBulletBase* AHComplexBullet::CloneTo_Implementation(FVector cloneLocation, FRotator cloneRot)
{
	return Super::CloneTo_Implementation(cloneLocation, cloneRot);
}

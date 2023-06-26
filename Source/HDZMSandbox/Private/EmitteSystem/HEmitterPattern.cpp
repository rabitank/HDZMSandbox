// Fill out your copyright notice in the Description page of Project Settings.


#include "EmitteSystem/HEmitterPattern.h"
#include "EmitteSystem/HSender.h"



void AHEmitterPattern::BeginPlay()
{
	Super::BeginPlay();
}

void AHEmitterPattern::Tick(float DeltaTime)
{

}

void AHEmitterPattern::InitPattern()
{
	Super::InitPattern();

	

	int insNum =  SendersIns.Num();
	float intervalAngular = 360.f / insNum;
	float offsetAngular{ 0.f };
	
	if (!(insNum % 2))
	{
		offsetAngular = intervalAngular / 2;

	}

	const float senderLength{ 20.f };

	for (int i = 0; i < insNum; i++)
	{
		auto senderIns = SendersIns[i];
		//设置位置等
		senderIns->InitSender(FQuat({ 1.f,0.f,0.f }, intervalAngular * i + offsetAngular).RotateVector(FVector(0.f, 0.f, senderLength)), FRotator(0.f));
	}

}

void AHEmitterPattern::PatternActive_Implementation()
{
	bIsActive = true;
	//temp. 
	SetActorScale3D(FVector(1.f));

}

void AHEmitterPattern::PatternRelax_Implementation()
{
	bIsActive = false;
	if(bIsShoot) StopShoot(GetInstigator());

	//temp
	SetActorScale3D(FVector(0.7f));
}

void AHEmitterPattern::StopShoot_Implementation(AActor* instagor)
{
	if (!bIsShoot) return;

	bIsShoot = false;
	for (AHSender* senderIns:SendersIns)
	{
		senderIns->CloseSender(instagor);
	}


}

bool AHEmitterPattern::Shoot_Implementation(AActor* instagor)
{
	if (!bIsActive)
	{
		UE_LOG(LogTemp, Warning, TEXT("发射模式正在休息,拒绝发射"));
		return false;
	}

	bIsShoot = true;

	for (AHSender* senderIns:SendersIns)
	{
		senderIns->OpenSender(instagor);
	}

	return true;

}

void AHEmitterPattern::UpdateSendersTransform_Implementation()
{
	int insNum = SendersIns.Num();
	float intervalAngular = 360.f / insNum;
	float offsetAngular{ 0.f };

	if (!(insNum % 2))
	{
		offsetAngular = intervalAngular / 2;

	}


	for (int i = 0; i < insNum; i++)
	{
		auto senderIns = SendersIns[i];
		//设置位置等
		senderIns->InitSender(FQuat({ 1.f,0.f,0.f }, intervalAngular * i + offsetAngular).RotateVector(FVector(0.f, 0.f, senderLength)), FRotator(0.f));
	}

}

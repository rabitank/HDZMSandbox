// Fill out your copyright notice in the Description page of Project Settings.


#include "EmitteSystem/HEmitterPattern.h"
#include "EmitteSystem/HSender.h"
#include "Kismet/KismetMathLibrary.h"



void AHEmitterPattern::BeginPlay()
{
	Super::BeginPlay();
}

void AHEmitterPattern::InitOffsetAngular()
{
	int insNum = SendersIns.Num();
	IntervalAngular = 360.f / insNum;
	if (!(insNum % 2))
	{
		bIsNeedOffset = true;
	}
}

void AHEmitterPattern::SetSenderLengths(float length)
{
	for (int i = 0; i < SendersIns.Num(); i++)
	{
		auto senderIns = SendersIns[i];
		//设置位置等

		auto relativeDirection = senderIns->RootComponent->GetRelativeLocation();
		relativeDirection.Normalize();

		senderIns->SetActorRelativeLocation(relativeDirection *length);

	}
}

void AHEmitterPattern::SetSenderRotateTo(float length)
{
	for (int i = 0; i < SendersIns.Num(); i++)
	{
		auto senderIns = SendersIns[i];
		//设置位置等

		auto relativeDirection = senderIns->RootComponent->GetRelativeLocation();
		relativeDirection.X = -length;
		senderIns->SetActorRelativeRotation(UKismetMathLibrary::MakeRotFromX(-relativeDirection));

	}
}

void AHEmitterPattern::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHEmitterPattern::InitPattern()
{
	Super::InitPattern();
	InitOffsetAngular();

	const float offsetAngular{ bIsNeedOffset ? IntervalAngular/2.f: 0.f };

	for (int i = 0; i < SendersIns.Num(); i++)
	{
		auto senderIns = SendersIns[i];
		//设置位置等
		//senderIns->InitSender(FQuat({ 1.f,0.f,0.f }, FMath::DegreesToRadians(IntervalAngular* i + offsetAngular)).RotateVector(FVector(0.f, 0.f, SenderLength)), FRotator(0.f));
		senderIns->InitSender(FQuat({ 1.f,0.f,0.f }, FMath::DegreesToRadians(IntervalAngular* i + offsetAngular)).RotateVector(FVector(0.f, 0.f, SenderLength)), FRotator(0.f));

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
		senderIns->InitSender(FQuat({ 1.f,0.f,0.f }, intervalAngular * i + offsetAngular).RotateVector(FVector(0.f, 0.f, SenderLength)), FRotator(0.f));
	}

}

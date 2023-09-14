// Fill out your copyright notice in the Description page of Project Settings.


#include "EmitteSystem/HSender.h"
#include "Kismet/KismetMathLibrary.h"
#include "EmitteSystem/HSenderPattern.h"
#include "LineSTG/HBulletBase.h"

// Sets default values
AHSender::AHSender()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ComRoot = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(ComRoot);

	ComMesh = CreateDefaultSubobject<UStaticMeshComponent>("SenderMesh");
	ComMesh->SetupAttachment(RootComponent);

	ComMesh->SetEnableGravity(false);
	ComMesh->bIgnoreRadialForce = true;
	ComMesh->bIgnoreRadialImpulse = true;
	ComMesh->SetCollisionProfileName("Sender");
	ComMesh->SetGenerateOverlapEvents(false);
	
}
AHBulletBase* AHSender::SpawnBullet(FVector loc,FRotator dir) const
{
	if(IsValid(SampleBullet))
	{
		//DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FSenderSpawnBulletDelegate, AHSenderPattern*, emittePatternIns, FName, bulletName, AActor*, playerInstigator);
		return SampleBullet->CloneTo(loc,dir);
	}
	UE_LOG(LogTemp,Warning,TEXT("SpawnBulletFaild:%s's samplebullet isn't valid"), *this->GetName());
	return nullptr;
}

// Called when the game starts or when spawned
void AHSender::BeginPlay()
{
	Super::BeginPlay();
}

void AHSender::OpenSender(AActor* instagor)
{
	bOpenShooting = true;
}

void AHSender::CloseSender(AActor* instagor)
{
	bOpenShooting = false;
}

/// <summary>
/// controled by initsetings. And Need 使用DefferSpawn时的初始化.
/// </summary>
void AHSender::InitSender(AHBulletBase* sample)
{
	CurRelaxTime = 0.f;
	Interval = 0.2f;
	bOpenShooting = false;
	ResetSampleBullet(sample);
}
void AHSender::InitSender_IPR(float interval,FVector relativePositon, FRotator relativeRot)
{
	Interval = interval;
	CurRelaxTime = interval;
	bOpenShooting = false;
	SetActorTransform( {relativeRot, relativePositon},false);
}
void AHSender::InitSender_IS(float interval,AHBulletBase* sample)
{
	ResetSampleBullet(sample);
	Interval = interval;
}

void AHSender::SetSenderRelativeTransform(FVector relativePositon, FRotator relativeRot)
{
	//remenber set Parent Actor;
	SetActorRelativeLocation(relativePositon, false);
	SetActorRelativeRotation(relativeRot.Quaternion(), false);
}

// Called every frame
void AHSender::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateSenderBehaviour(DeltaTime);
}

void AHSender::Shooting_Implementation()
{
	SpawnBullet(GetActorLocation(),GetActorRotation());
}

void AHSender::ResetInterval(float newInterval)
{
	const float hasp{Interval - CurRelaxTime};
	CurRelaxTime = newInterval - hasp;
	if (CurRelaxTime<0)
	{
		CurRelaxTime = -0.001;
	}
	Interval = newInterval;
}

void AHSender::ResetSampleBullet(AHBulletBase* sample)
{
	if(IsValid(sample))
	{
		SampleBullet = sample;
	}
}

void AHSender::UpdateSenderBehaviour_Implementation(float DeltaTime)
{
	//AddActorLocalRotation(FRotator{ 0.f,SenderSettings->SenderAngularVelocaity * DeltaTime,0.f }.Quaternion() , false);
	if (bOpenShooting)
	{
		CurRelaxTime -= DeltaTime;
		if (CurRelaxTime < 0)
		{
			Shooting();
			CurRelaxTime += Interval;
		}
	}
}



// Fill out your copyright notice in the Description page of Project Settings.


#include "EmitteSystem/HSender.h"
#include "EmitteSystem/HSenderSettings.h"
#include "Kismet/KismetMathLibrary.h"

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

void AHSender::SpawnBullet()
{

	for (auto atrace : SenderSettings->BulletTraces)
	{
		FActorSpawnParameters bulletPawnPars;
		bulletPawnPars.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		bulletPawnPars.Instigator = GetInstigator();


		AHBulletBase* bullet =Cast<AHBulletBase>(GetWorld()->SpawnActor<AActor>(SenderSettings->BulletType,GetActorLocation(), UKismetMathLibrary::ComposeRotators(GetActorRotation(), atrace), bulletPawnPars));
		if (bRecoverBulletBehaviour)  InitBullet(bullet);
	
	}
	

}

// Called when the game starts or when spawned
void AHSender::BeginPlay()
{
	Super::BeginPlay();


	check(IsValid(SenderSettings));
	check(IsValid(SenderSettings->BulletType));


	if(!bWorldSender) InitSender();

	CurRelaxTime = SenderSettings->Interval;


}



void AHSender::OpenSender(AActor* instagor)
{
	bOpenShooting = true;
	CurRelaxTime = SenderSettings->Interval;
}

void AHSender::CloseSender(AActor* instagor)
{
	bOpenShooting = false;
}

/// <summary>
/// controled by initsetings
/// </summary>
void AHSender::InitSender()
{

	//remenber set Parent Actor;
	SetActorRelativeLocation(SenderSettings->SenderInitPosition, false);
	SetActorRelativeRotation(SenderSettings->SenderInitRotator.Quaternion(), false);
}

/// <summary>
/// controlled by Patterns
/// </summary>
/// <param name="relativePositon"></param>
/// <param name="relativeRot"></param>
void AHSender::InitSender(FVector relativePositon, FRotator relativeRot)
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


void AHSender::UpdateSenderBehaviour_Implementation(float DeltaTime)
{
	//AddActorLocalRotation(FRotator{ 0.f,SenderSettings->SenderAngularVelocaity * DeltaTime,0.f }.Quaternion() , false);
	if (bOpenShooting)
	{
		CurRelaxTime -= DeltaTime;
		if (CurRelaxTime < 0)
		{
			Shooting();
			CurRelaxTime += SenderSettings->Interval;
		}
	}

}

//
void AHSender::Shooting()
{
	SpawnBullet();
}


void AHSender::InitBullet_Implementation(AHBulletBase* bullet)
{
	bullet->SetLinearVelocity(BulletVelocity);
	bullet->SetLinearAcceleration(BulletVelAce);
	bullet->SetAngularVeloctiy(BulletAngVel);
	bullet->SetAngularAcceleration(BulletAngAce);

};


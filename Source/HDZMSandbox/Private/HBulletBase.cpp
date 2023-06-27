// Fill out your copyright notice in the Description page of Project Settings.


#include "HBulletBase.h"
#include "GameFramework/Actor.h"
#include "HPlayerCharacter.h"
#include "../HDZMSandbox.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
AHBulletBase::AHBulletBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	ComRoot = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(ComRoot);

	ComMesh = CreateDefaultSubobject<UStaticMeshComponent>("BulletMesh");
	ComMesh->SetupAttachment(RootComponent);

	//Created CollisionProfile "Projectile" in Engine-Collision
	ComMesh->SetCollisionProfileName("Bullet");
	ComMesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	ComMesh->OnComponentHit.AddDynamic(this, &AHBulletBase::OnActorHit);

	//collision APi etc:
	//ComSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	//ComSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	//more information view:https://www.unrealengine.com/zh-CN/blog/collision-filtering
	
	bCanFadeAway = false;

//	ComEffectParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SProComParticleSystem"));
//	ComEffectParticle->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AHBulletBase::BeginPlay()
{
	Super::BeginPlay();
	if(bCanFadeAway)
		GetWorld()->GetTimerManager().SetTimer(TimeHandle_FadeAway,this,&AHBulletBase::FadeAway,LifeDuration,false);
	else
		GetWorld()->GetTimerManager().SetTimer(TimeHandle_FadeAway,this,&AHBulletBase::Exploed,LifeDuration,false);

}

void AHBulletBase::OnActorHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& hit)
{

	//temp 
	//@TODO: 将来交给Manager管理.
	if (OtherActor && OtherActor == GetInstigator())
	{
		return;
	}
	Exploed();
}


void AHBulletBase::FadeAway_Implementation()
{
	//if call fadeAway in error;
	if (!ensureMsgf(bCanFadeAway,TEXT("Don't Call FaedAway: bCanFadeAway is false!")))
	{
		Exploed();
		return;
	}

	LogOnScreen(this,TEXT("a Bullet FadeAway!"),FColor::White,1.f);
	if (ensure(!IsPendingKill()))
	{
		Destroy();
	}
}

void AHBulletBase::Exploed_Implementation()
{
	LogOnScreen(this,TEXT("a Bullet Exploed!"),FColor::White,1.f);
	if (ensure(!IsPendingKill()))
	{
		Destroy();
	}

}

// Called every frame
void AHBulletBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateAccelreation(DeltaTime);
	UpdateVelcoity(DeltaTime);

	AddActorLocalOffset(FVector::ForwardVector *LinearVelocity * DeltaTime, false);
	AddActorLocalRotation(FQuat(FVector::UpVector,AngularVelocity*DeltaTime),false);

}

void AHBulletBase::UpdateVelcoity(float DeltaTime)
{

	LinearVelocity = FMath::Clamp(LinearVelocity + LinearAcceleration * DeltaTime, -MaxLinearVeclocity, MaxLinearVeclocity);
	AngularVelocity += AngularAcceleration * DeltaTime;

}



// Fill out your copyright notice in the Description page of Project Settings.


#include "HBulletBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Actor.h"
#include "HPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../HDZMSandbox.h"

// Sets default values
AHBulletBase::AHBulletBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ComSphere = CreateDefaultSubobject<USphereComponent>(TEXT("HBulComSphere"));
	RootComponent = ComSphere;

	//Created CollisionProfile "Projectile" in Engine-Collision
	ComSphere->SetCollisionProfileName("Projectile");
	ComSphere->OnComponentHit.AddDynamic(this, &AHBulletBase::OnActorHit);

	//collision APi etc:
	//ComSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	//ComSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	//more information view:https://www.unrealengine.com/zh-CN/blog/collision-filtering

	ComMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("HBulComProjectileMovement"));
	ComMovement->bRotationFollowsVelocity = true;
	ComMovement->bInitialVelocityInLocalSpace = true;
	ComMovement->ProjectileGravityScale = 0.f;

	ComMovement->bAutoActivate = true;

	InitialSpeed = 8000.f;
	ComMovement->InitialSpeed = InitialSpeed;
	RecoilForce = 1000000.f;

	CurrentContainEnergy = 10.f;
	
	LifeDuration = 10.f;
	bCanFadeAway = false;

//	ComEffectParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SProComParticleSystem"));
//	ComEffectParticle->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AHBulletBase::BeginPlay()
{
	Super::BeginPlay();
	FlyStartedTime = GetWorld()->GetTimeSeconds();
	ApplyRecoilForce();
	if(bCanFadeAway)
		GetWorld()->GetTimerManager().SetTimer(TimeHandle_FadeAway,this,&AHBulletBase::FadeAway,LifeDuration,false);
	else
		GetWorld()->GetTimerManager().SetTimer(TimeHandle_FadeAway,this,&AHBulletBase::Exploed,LifeDuration,false);

}

void AHBulletBase::ApplyRecoilForce_Implementation()
{
	APawn* InsPawn = Cast<APawn>(GetInstigator());
	if (ensure(InsPawn))
	{
		AHPlayerCharacter* Player = Cast<AHPlayerCharacter>(InsPawn);
		if (Player)
		{
			UCharacterMovementComponent* CompCharacterMove = Cast<UCharacterMovementComponent>(Player->GetComponentByClass(UCharacterMovementComponent::StaticClass()));
			if (CompCharacterMove)
			{

				FRotator rot = Player->GetController()->GetControlRotation();
				CompCharacterMove->AddImpulse( - rot.Vector() * RecoilForce);
			}
		}
	}
}

void AHBulletBase::OnActorHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& hit)
{
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

}


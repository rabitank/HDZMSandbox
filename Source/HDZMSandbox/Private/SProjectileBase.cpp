// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectileBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASProjectileBase::ASProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ComSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SProComSphere"));
	RootComponent = ComSphere;

	//Created CollisionProfile "Projectile" in Engine-Collision
	ComSphere->SetCollisionProfileName("Projectile");
	ComSphere->OnComponentHit.AddDynamic(this, &ASProjectileBase::OnActorHit);

	//collision APi etc:
	//ComSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	//ComSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	//more information view:https://www.unrealengine.com/zh-CN/blog/collision-filtering

	ComMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("SProComProjectileMovement"));
	ComMovement->InitialSpeed = 8000.f;
	ComMovement->bRotationFollowsVelocity = true;
	ComMovement->bInitialVelocityInLocalSpace = true;
	ComMovement->ProjectileGravityScale = 0.f;

	ComEffectParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SProComParticleSystem"));
	ComEffectParticle->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASProjectileBase::BeginPlay()
{
	Super::BeginPlay();
}

void ASProjectileBase::OnActorHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& hit)
{
	//Explode has been defineded in generate.h , just override _Implementation
	Explode();
}


//_Implementation from "BlueprintNativeEvent"!!!

void ASProjectileBase::Explode_Implementation()
{
	//makesure is ready to dead(Destroy)
	if (ensure(!IsPendingKill()))
	{
		if (ensure(ExplodeParticle))
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplodeParticle, GetActorLocation(), GetActorRotation());
		}
		Destroy();


	}
}

// Called every frame
void ASProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


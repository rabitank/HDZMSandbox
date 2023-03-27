// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ComSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SMagComSphere"));
	RootComponent = ComSphere;


	//Created CollisionProfile "Projectile" in Engine-Collision
	ComSphere->SetCollisionProfileName("Projectile");

	//collision APi etc:
	//ComSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	//ComSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	//more information view:https://www.unrealengine.com/zh-CN/blog/collision-filtering

	ComMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("SMagComProjectileMovement"));
	ComMovement->InitialSpeed = 1000.f;
	ComMovement->bRotationFollowsVelocity = true;
	ComMovement->bInitialVelocityInLocalSpace= true;
	ComMovement->ProjectileGravityScale = 0.f;

	ComEffectParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SMagComParticleSystem"));
	ComEffectParticle->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


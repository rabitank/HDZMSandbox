// Fill out your copyright notice in the Description page of Project Settings.


#include "SBlackhole.h"
#include "SMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ASBlackhole::ASBlackhole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//collision APi etc:
	//ComSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	//ComSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	//more information view:https://www.unrealengine.com/zh-CN/blog/collision-filtering

	ComMovement->InitialSpeed = 2000.f;

	//ComEffectParticle->bAutoActivate = false;

	//use comExplodForce to simulate BlackHole
	ComExplodForce = CreateDefaultSubobject<URadialForceComponent>("SBlaComRadialForce");
	ComExplodForce->SetupAttachment(RootComponent);
	ComExplodForce->Radius = 300.f;
	ComExplodForce->SetAutoActivate(false);
	ComExplodForce->bImpulseVelChange = true;
	ComExplodForce->AddCollisionChannelToAffect(ECC_WorldDynamic);
	ComExplodForce->AddCollisionChannelToAffect(ECC_PhysicsBody);
}

void ASBlackhole::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor)
	OtherActor->Destroy();
}

// Called when the game starts or when spawned
void ASBlackhole::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASBlackhole::PostInitializeComponents()
{
		Super::PostInitializeComponents();
		//handler should be UFUNCTION();
		//Bind in consturctfunction may be error in hotreloading. Binding in BeginPlay or PostInitializeComponents
		
		//has done in BP
		//ComBlackholeSphere->OnComponentBeginOverlap.AddDynamic(this, &ASBlackhole::OnActorOverlap);

		//TODO: fix actor didn't destoryed!;

}

// Called every frame
void ASBlackhole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


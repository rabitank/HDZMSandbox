// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplodingBarre.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ASExplodingBarre::ASExplodingBarre()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ComBarreMesh = CreateDefaultSubobject<UStaticMeshComponent>("SExBComStaticMesh");
	RootComponent = ComBarreMesh;
	ComBarreMesh->SetSimulatePhysics(true);

	ComExplodForce = CreateDefaultSubobject<URadialForceComponent>("SExBComRadialForce");
	//if not set, it will be init at world origin point;
	ComExplodForce->SetupAttachment(RootComponent);
	ComExplodForce->bImpulseVelChange = true;



	//handler should be UFUNCTION();
	ComBarreMesh->OnComponentHit.AddDynamic(this, &ASExplodingBarre::FireImpulseForce);




}

// Called when the game starts or when spawned
void ASExplodingBarre::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASExplodingBarre::FireImpulseForce(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ComExplodForce->FireImpulse();
}

// Called every frame
void ASExplodingBarre::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


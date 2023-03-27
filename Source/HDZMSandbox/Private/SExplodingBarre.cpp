// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplodingBarre.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "DrawDebugHelpers.h"

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
	ComExplodForce->Radius = 300.f;
	ComExplodForce->SetAutoActivate(false);
	ComExplodForce->bImpulseVelChange = true;

	ComExplodForce->AddCollisionChannelToAffect(ECC_WorldDynamic);

}

// Called when the game starts or when spawned
void ASExplodingBarre::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASExplodingBarre::PostInitializeComponents()
{
	//Call parent method!
	Super::PostInitializeComponents();
	
	//handler should be UFUNCTION();
	//Bind in consturctfunction may be error in hotreloading. Binding in BeginPlay or PostInitializeComponents
	ComBarreMesh->OnComponentHit.AddDynamic(this, &ASExplodingBarre::OnActorHit);
}

void ASExplodingBarre::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	FireImpulseForce();

	UE_LOG(LogTemp, Log, TEXT("FireImpulseForce in ExplodiongBarre"));
	FString comBindedStr = FString::Printf(TEXT("Hit at Location :%s"), *Hit.ImpactPoint.ToString());
	DrawDebugString(GetWorld(), Hit.ImpactPoint, comBindedStr, NULL, FColor::Green, 2.f, false);
	//more how debugView: https://nerivec.github.io/old-ue4-wiki/pages/logs-printing-messages-to-yourself-during-runtime.html
}

void ASExplodingBarre::FireImpulseForce()
{
	ComExplodForce->FireImpulse();
}

// Called every frame
void ASExplodingBarre::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Fill out your copyright notice in the Description page of Project Settings.


#include "SDashPorjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASDashPorjectile::ASDashPorjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ComSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SDasComSphere"));
	RootComponent = ComSphere;


	//Created CollisionProfile "Projectile" in Engine-Collision
	ComSphere->SetCollisionProfileName("Projectile");

	//collision APi etc:
	//ComSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	//ComSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	//more information view:https://www.unrealengine.com/zh-CN/blog/collision-filtering

	ComMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("SDasComProjectileMovement"));
	ComMovement->InitialSpeed = 1000.f;
	ComMovement->bRotationFollowsVelocity = true;
	ComMovement->bInitialVelocityInLocalSpace = true;
	ComMovement->ProjectileGravityScale = 0.f;

	ComEffectParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SDasComParticleSystem"));
	ComEffectParticle->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASDashPorjectile::BeginPlay()
{
	Super::BeginPlay();

	ComSphere->IgnoreActorWhenMoving(GetInstigator(),true);

	GetWorldTimerManager().SetTimer(TimerHandle_Flytime,this,&ASDashPorjectile::DashInstigatorAndDestory,0.8f);
	
}

void ASDashPorjectile::DashInstigatorAndDestory()
{
	//dash
	GetInstigator()->SetActorLocation(this->GetActorLocation());
	//stop
	ComMovement->SetVelocityInLocalSpace(FVector(0.f));

	FTimerHandle TimerHandel_destroy;
	GetWorldTimerManager().SetTimer(TimerHandel_destroy, 0.2f, false);
	GetWorldTimerManager().ClearTimer(TimerHandel_destroy);
	
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExploseParticle,GetActorLocation(),GetActorRotation());
	Destroy();
}


void ASDashPorjectile::PostInitializeComponents()
{
	//Call SuperMethod!!!!!!!!!!!!!
	Super::PostInitializeComponents();
	ComSphere->OnComponentHit.AddDynamic(this,&ASDashPorjectile::OnActorHit);
}

// Called every frame
void ASDashPorjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASDashPorjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	DashInstigatorAndDestory();
}


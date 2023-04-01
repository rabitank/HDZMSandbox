// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include <SAttributeComponent.h>
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ComSphere->OnComponentBeginOverlap.AddDynamic(this,&ASMagicProjectile::OnActorOverlap);
	ComAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("SMagComAudio"));
	ComAudio->SetupAttachment(RootComponent);
	DamageAmount = -20.f;
}
void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor!= GetInstigator()) //not nullptr
	{
		//StaticClass() getType in UE? = typeof()? ,return UClass*!!!
		//GetComponentByClass: iterator all comps untile meet the same types comp
		USAttributeComponent* comAttribute =(USAttributeComponent*) OtherActor->GetComponentByClass(USAttributeComponent::StaticClass());
		if (comAttribute)
		{
			comAttribute->ApplyHealthChangeDelta(DamageAmount);
			Destroy();
		}
	}

}


void ASMagicProjectile::Explode_Implementation()
{
	if (ensure(!IsPendingKill()))
	{
		if (ensure(ExplodeParticle))
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(),ImpactAudio,GetActorLocation());
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplodeParticle, GetActorLocation(), GetActorRotation());
		}
		Destroy();
	}
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


// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include <SAttributeComponent.h>
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "SGameModeFunctionLibrary.h"
#include "SActionComponent.h"
#include "SAction.h"
#include "SActionEffect.h"

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
	
	//static FGameplayTag ParryTag =  
	
	if (OtherActor && OtherActor != GetInstigator())
	{

		USActionComponent* actionCom = Cast<USActionComponent>( OtherActor->GetComponentByClass(USActionComponent::StaticClass()));
		if (actionCom && actionCom->ActiveGameplayTags.HasTag(ParryTag))
		{
			ComMovement->Velocity = -ComMovement->Velocity;
			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}

		if (USGameModeFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, DamageAmount,SweepResult))
		{
			//super  method
			Explode();
			if(actionCom && OtherActor->HasAuthority())
				if(BurningEffectClass)
					actionCom->AddAction(GetInstigator(), BurningEffectClass);
			return;
		};

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


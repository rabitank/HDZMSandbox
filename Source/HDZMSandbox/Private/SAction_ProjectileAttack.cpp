// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_ProjectileAttack.h"
#include "SCharacter.h"
#include "Kismet/GameplayStatics.h"
#include <Kismet/KismetMathLibrary.h>
#include <DrawDebugHelpers.h>
#include "SAttributeComponent.h"

void USAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, "projectileAttack: Part On Action");

	ASCharacter* character = Cast<ASCharacter>(Instigator);
	if (character)
	{
		if (CostRage >0.f)
		{

			if (!Cast<USAttributeComponent>(character->GetComponentByClass(USAttributeComponent::StaticClass()))
				->ApplyRageChangeDelta(-CostRage, character))
			{
				UE_LOG(LogTemp,Warning,TEXT("No enough Rage to do this Action"))
				StopAction(Instigator);
				return;
			}
		}


		character->PlayAnimMontage(AttackAnim);
		if(ThrowMagicProEffect)
			UGameplayStatics::SpawnEmitterAttached(ThrowMagicProEffect,character->GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
		//UGameplayStatics::PlayWorldCameraShake(this, CamerShake,GetActorLocation(),20.f,100.f,10.f);
		if(CamerShakeAttack)
			UGameplayStatics::PlayWorldCameraShake(GetOuter(),CamerShakeAttack, Cast<AActor>(GetOuter())->GetActorLocation(),20.f,100.f);
		
		//only actually elaps proj in server
		//and server's copy proj to show in client
		if (character->HasAuthority())
		{
			FTimerHandle TimeHandle_PrimaryAttack;
			FTimerDelegate delegate;
			delegate.BindUFunction(this, "AttackDelay_Elapsed", Instigator);

			character->GetWorldTimerManager().SetTimer(TimeHandle_PrimaryAttack, delegate, AttackDelay,false);
		}

	}


}

USAction_ProjectileAttack::USAction_ProjectileAttack()
{
	HandSocketName = TEXT("Muzzle_01");
	AttackDelay = 0.2f;

	CostRage = 0.f;
}

void USAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* Instigator)
{

	if (ensureAlways(Instigator))
	{
		FTransform SMagTM;

		FVector handLocation = Instigator->GetMesh()->GetSocketLocation(TEXT("Muzzle_01"));

		FCollisionShape shape;
		shape.SetSphere(20.f);

		FCollisionQueryParams params;
		params.AddIgnoredActor(Instigator);
		FCollisionObjectQueryParams queryParams(FCollisionObjectQueryParams::AllDynamicObjects);
		queryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		queryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		queryParams.AddObjectTypesToQuery(ECC_Pawn);

		float radians = 50000.f;
		FVector start = Instigator->GetPawnViewLocation()+ Instigator->GetControlRotation().Vector() * 50.f;
		FVector end = start + Instigator->GetControlRotation().Vector() * radians;


		FHitResult hitResult;

		bool isSuccessed = GetWorld()->SweepSingleByObjectType(hitResult, start, end, FQuat::Identity, queryParams, shape, params);
		if (isSuccessed)
		{
			end = hitResult.ImpactPoint;

		}

		FRotator projectileRot = UKismetMathLibrary::MakeRotFromX(end - handLocation);
		SMagTM = FTransform(projectileRot, handLocation);
		DrawDebugSphere(GetWorld(), hitResult.Location, 30.f, 12, FColor::Yellow, false, 2.f);


		FActorSpawnParameters SMagspawnPars;
		SMagspawnPars.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SMagspawnPars.Instigator = Instigator;

		GetWorld()->SpawnActor<AActor>(MagicProjectile, SMagTM, SMagspawnPars);
	}

	StopAction(Instigator);
}

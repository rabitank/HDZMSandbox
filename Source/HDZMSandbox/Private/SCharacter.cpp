// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include <SInteractionComponent.h>
#include "Animation/AnimMontage.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include <SAttributeComponent.h>
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"


// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//foue chr of class name + Varname
	ComSpringArm = CreateDefaultSubobject<USpringArmComponent>("SChaComSprinArm");
	ComSpringArm->SetupAttachment(RootComponent);
	ComSpringArm->bUsePawnControlRotation = true;
	ComSpringArm->TargetArmLength = 350.f;

	ComCamera = CreateDefaultSubobject<UCameraComponent>("SChaComCamera");
	ComCamera->SetupAttachment(ComSpringArm);

	ComSAttribute = CreateDefaultSubobject<USAttributeComponent>("SChaComSAttribute");

	//make char rotate CharmainDirection  to movement direction
	GetCharacterMovement()->bOrientRotationToMovement = true; 

	ComSInteraction = CreateDefaultSubobject<USInteractionComponent>("SChaComSInteraction");

	//it used to be set true in default;
	bUseControllerRotationYaw = false; 

	DashDistance = 3.f;

	HandSocketName = TEXT("Muzzle_01");
	HitTimeParametersName = TEXT("HitTime");

}

void ASCharacter::OnHealthChanged(USAttributeComponent* owningComp, AActor* instigatorActor, float newHealth, float delta)
{
	if (delta < 0.f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(HitTimeParametersName, GetWorld()->GetTimeSeconds());
		GetMesh()->SetScalarParameterValueOnMaterials("HitSpeed", 3.f);
		GetMesh()->SetVectorParameterValueOnMaterials("FlashColor", FVector(0.9f, 0.f, 0.5f));
		//if die , can't control	
		if (newHealth <= 0.f )
		{
			APlayerController* controllerPC =Cast<APlayerController>(GetController());
			DisableInput(controllerPC);
		}

	}
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ComSAttribute->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);

}

//rotate to controller direction and move forward
void ASCharacter::MoveForward(float Val)
{
	FRotator controlRot = GetControlRotation();
	controlRot.Roll = 0.f;
	controlRot.Pitch= 0.f;
	// commit rotateVector to player comtroller;
	AddMovementInput(controlRot.Vector(), Val);


	//Axis X positive means the ForwardDirection;

}

void ASCharacter::MoveRight(float Val)
{
	//get the rightVector beases on controller

	FRotator controlRot = GetControlRotation();
	controlRot.Roll = 0.f;
	controlRot.Pitch= 0.f;

	//X means forward then Y means right;
	FVector rightVector = FRotationMatrix(controlRot).GetScaledAxis(EAxis::Y);
	AddMovementInput(rightVector, Val);
}



void ASCharacter::PrimaryInteraction()
{
	//ComSInteraction cant be nullptr; may need check?
	if(ComSInteraction)
	ComSInteraction->PrimaryInteraction();
}

void ASCharacter::Dash()
{
	PlayAnimMontage(DashAnim);
	GetWorldTimerManager().SetTimer(TimeHandle_Dash,this,&ASCharacter::Dash_Elapsed,0.2f);
}

void ASCharacter::Dash_Elapsed()
{
//then The DashPorClass will make the instigator move to it
	SpawnProjectile(DashPorClass);
}
void ASCharacter::PrimaryAttack()
{

	PlayAnimMontage(AttackAnim);

	UGameplayStatics::SpawnEmitterAttached(ThrowMagicProEffect,GetMesh(), HandSocketName,FVector::ZeroVector,FRotator::ZeroRotator,EAttachLocation::SnapToTarget);
	//UGameplayStatics::PlayWorldCameraShake(this, CamerShake,GetActorLocation(),20.f,100.f,10.f);
	Cast<APlayerController>(GetController())->ClientPlayCameraShake(CamerShake);
	
	GetWorldTimerManager().SetTimer(TimeHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_Elapsed, 0.2f);
	// recallBackFunction: PrimaryAttack_Elapsed
}

void ASCharacter::PrimaryAttack_Elapsed()
{
	SpawnProjectile(MagicProjectile);

}

void ASCharacter::BlackholeAttack()
{
	SpawnProjectile(BlackholeClass);
}

void ASCharacter::SpawnProjectile(TSubclassOf<AActor> projectileType)
{
	//ensure:
	//assert in ue? or just a temp stop, you can continue
	//and it wouldnot appare in release Version
	//ensure: stop once if cant pass
	//ensureAlways:always stop if cant pass
	if (ensureAlways(MagicProjectile))
	{ 
		FTransform SMagTM;

		FVector handLocation = GetMesh()->GetSocketLocation(TEXT("Muzzle_01"));

		FCollisionShape shape;
		shape.SetSphere(20.f);

		FCollisionQueryParams params;
		params.AddIgnoredActor(this);
		FCollisionObjectQueryParams queryParams(FCollisionObjectQueryParams::AllDynamicObjects);
		queryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		queryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		queryParams.AddObjectTypesToQuery(ECC_Pawn);

		float radians = 50000.f;
		FVector start = ComCamera->GetComponentLocation() + ComCamera->GetComponentRotation().Vector()*50.f;
		FVector end = start + ComCamera->GetComponentRotation().Vector() * radians;
		

		FHitResult hitResult;

		bool isSuccessed = GetWorld()->SweepSingleByObjectType(hitResult, start, end,FQuat::Identity, queryParams,shape,params);
		if (isSuccessed)
		{
			end = hitResult.ImpactPoint;

		}

		FRotator projectileRot = UKismetMathLibrary::MakeRotFromX(end - handLocation);
		SMagTM = FTransform(projectileRot, handLocation);
		DrawDebugSphere(GetWorld(), hitResult.Location, 30.f, 12, FColor::Yellow, false, 2.f);
		

		FActorSpawnParameters SMagspawnPars;
		SMagspawnPars.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SMagspawnPars.Instigator = this;

		GetWorld()->SpawnActor<AActor>(projectileType, SMagTM, SMagspawnPars);
	}
}




// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);


	//AddControllerYawInput at Apawn
	//you should open "Use Pawn Control Rotation" at springArmCom to allow playerController to controll arm
	PlayerInputComponent->BindAxis("MouseX", this, &ASCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("MouseY", this, &ASCharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack",IE_Pressed,this,&ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&ASCharacter::Jump);
	
	//you cant directly call componentMethod;
	PlayerInputComponent->BindAction("PrimaryInteract",IE_Pressed, this, &ASCharacter::PrimaryInteraction);

	//bind E
	PlayerInputComponent->BindAction("BlackholeAttack",IE_Pressed, this, &ASCharacter::BlackholeAttack);

	PlayerInputComponent->BindAction("Dash",IE_Pressed, this, &ASCharacter::Dash);
	
}


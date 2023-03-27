// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include <SInteractionComponent.h>
#include "Animation/AnimMontage.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"


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

	//make char rotate CharmainDirection  to movement direction
	GetCharacterMovement()->bOrientRotationToMovement = true; 

	ComSInteraction = CreateDefaultSubobject<USInteractionComponent>("SChaComSInteraction");

	//it used to be set true in default;
	bUseControllerRotationYaw = false; 

	DashDistance = 3.f;
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

void ASCharacter::PrimaryAttack()
{

	PlayAnimMontage(AttackAnim);
	GetWorldTimerManager().SetTimer(TimeHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_Elapsed, 0.2f);
	// recallBackFunction: PrimaryAttack_Elapsed
}

void ASCharacter::PrimaryAttack_Elapsed()
{
	FTransform SMagTM;
	bool  isSuccessed = GetProjectileAttackTM(SMagTM);

	FActorSpawnParameters SMagspawnPars;
	SMagspawnPars.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SMagspawnPars.Instigator = this;

	GetWorld()->SpawnActor<AActor>(PorjectileClass, SMagTM, SMagspawnPars);
}

void ASCharacter::PrimaryInteraction()
{
	//ComSInteraction cant be nullptr; may need check?
	ComSInteraction->PrimaryInteraction();
}

void ASCharacter::Dash()
{
	PlayAnimMontage(DashAnim);
	GetWorldTimerManager().SetTimer(TimeHandle_Dash,this,&ASCharacter::Dash_Elapsed,0.2f);
}

void ASCharacter::Dash_Elapsed()
{
	FTransform SMagTM;
	bool  isSuccessed = GetProjectileAttackTM(SMagTM);

	FActorSpawnParameters SMagspawnPars;
	SMagspawnPars.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SMagspawnPars.Instigator = this;

	GetWorld()->SpawnActor<AActor>(DashPorClass, SMagTM, SMagspawnPars);

	//then The DashPorClass will make the instigator move to it
}

bool ASCharacter::GetProjectileAttackTM(FTransform& SprojectileTM)
{
	FVector handLocation = GetMesh()->GetSocketLocation(TEXT("Muzzle_01"));

	FHitResult hitResult;
	float radians = 10000.f;
	FVector start = ComCamera->GetComponentLocation();
	FVector end = start + ComCamera->GetComponentRotation().Vector() * radians;
	FCollisionObjectQueryParams queryParams(FCollisionObjectQueryParams::AllDynamicObjects);
	queryParams.AddObjectTypesToQuery(ECC_WorldStatic);

	bool isSuccessed = GetWorld()->LineTraceSingleByObjectType(hitResult, start, end, queryParams);
	if (isSuccessed)
	{
		FVector targetLocation = hitResult.Location;
		FRotator projectileRot = UKismetMathLibrary::MakeRotFromX(targetLocation - handLocation);

		SprojectileTM = FTransform(projectileRot, handLocation);
		DrawDebugSphere(GetWorld(), hitResult.Location, 30.f, 12, FColor::Yellow, false, 2.f);
	}
	else
	{
		SprojectileTM = FTransform(GetControlRotation(), GetMesh()->GetSocketLocation(TEXT("Muzzle_01")));
	}
	return isSuccessed;
}

void ASCharacter::BlackholeAttack()
{
	FTransform SMagTM;
	bool  isSuccessed = GetProjectileAttackTM(SMagTM);

	FActorSpawnParameters SMagspawnPars;
	SMagspawnPars.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SMagspawnPars.Instigator = this;

	GetWorld()->SpawnActor<AActor>(BlackholeClass, SMagTM, SMagspawnPars);
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


// Fill out your copyright notice in the Description page of Project Settings.


#include "HPlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../HDZMSandbox.h"

// Sets default values
AHPlayerCharacter::AHPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ComSpringArm = CreateDefaultSubobject<USpringArmComponent>("HPlaComSpringArm");
	ComSpringArm->SetupAttachment(GetRootComponent());
	ComSpringArm->TargetArmLength = 400.f;
	ComSpringArm->bUsePawnControlRotation = true;

	//Camera default value:
	//
	// 	bUseControllerViewRotation_DEPRECATED = true; // the previous default value before bUsePawnControlRotation replaced this var.
	// 	bCameraMeshHiddenInGame = true;
	// 
	// 	FieldOfView = 90.0f;
	// 	AspectRatio = 1.777778f;
	// 	OrthoWidth = 512.0f;
	// 	OrthoNearClipPlane = 0.0f;
	// 	OrthoFarClipPlane = WORLD_MAX;
	// 	bConstrainAspectRatio = false;
	// 	bUseFieldOfViewForLOD = true;
	// 	PostProcessBlendWeight = 1.0f;
	// 	bUsePawnControlRotation = false;
	// 	bAutoActivate = true;
	// 	bLockToHmd = true;

	ComCamera = CreateDefaultSubobject<UCameraComponent>("HPlaComCamera");
	ComCamera->SetupAttachment(ComSpringArm);
	ComCamera->SetFieldOfView(120);
	//test and set in BP
	//@check UCharacterMovementComponent .h 

	ComAttribute = CreateDefaultSubobject<UHAttributeComponent>("HPlaComAttribute");

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;


}

// Called when the game starts or when spawned
void AHPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}


void AHPlayerCharacter::MoveRight(float val)
{
	FRotator controlRot = GetControlRotation();
	controlRot.Roll = 0.f;
	controlRot.Pitch = 0.f;
	
	//X means forward then Y means right;
	FVector rightVector = FRotationMatrix(controlRot).GetScaledAxis(EAxis::Y);
	AddMovementInput(rightVector, val);

}


void AHPlayerCharacter::PrimaryEmitt()
{
	FActorSpawnParameters SMagspawnPars;
	SMagspawnPars.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SMagspawnPars.Instigator = this;

	LogOnScreen(this, "Primary Emitt");
	if(ensure(BulletClass))
		GetWorld()->SpawnActor<AActor>(BulletClass, GetTransform(), SMagspawnPars);
}

void AHPlayerCharacter::MoveForward(float val)
{
	FRotator controlRot = GetControlRotation();
	controlRot.Roll = 0.f;
	controlRot.Pitch = 0.f;
	// commit rotateVector to player comtroller;
	AddMovementInput(controlRot.Vector(), val);

}

// Called every frame
void AHPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AHPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AHPlayerCharacter::MoveRight);

	//AddControllerYawInput at Apawn
	//you should open "Use Pawn Control Rotation" at springArmCom to allow playerController to controll arm
	PlayerInputComponent->BindAxis("MouseX", this, &AHPlayerCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("MouseY", this, &AHPlayerCharacter::AddControllerPitchInput);
	
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed,this, &AHPlayerCharacter::Jump);
	
	PlayerInputComponent->BindAction("PrimaryAttack",IE_Pressed,this,&AHPlayerCharacter::PrimaryEmitt);


}


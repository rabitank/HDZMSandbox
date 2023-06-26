// Fill out your copyright notice in the Description page of Project Settings.


#include "HPlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../HDZMSandbox.h"
#include "HAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "HEmitterComponent.h"
#include "HActionComponent.h"
#include "Components/ArrowComponent.h"

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


	ComActions = CreateDefaultSubobject<UHActionComponent>("HPlaComAction");
	ComAttribute = CreateDefaultSubobject<UHAttributeComponent>("HPlaComAttribute");

	ComEmitter = CreateDefaultSubobject<UHEmitterComponent>("HPlaComEmitter");

	ComSphereCollision = CreateDefaultSubobject<USphereComponent>("HPlaComSphereCollision");
	ComSphereCollision->SetupAttachment(GetRootComponent());

	ComEmitterMoveController  = CreateDefaultSubobject<USphereComponent>("HPlaComEmitterMoveController");
	ComEmitterMoveController->SetupAttachment(GetRootComponent());

	ComEmitterLocation = CreateDefaultSubobject<USceneComponent>("HPlaComEmiiterPostion");
	ComEmitterLocation->SetupAttachment(ComEmitterMoveController);

	EmitterMoveRadiance = 150.f;
	EmitterMoveOffSet = FVector(0.f,0.f,7.f);
	EmitterDirectionOffSet = FRotator(0.f,10.f,10.f);

	ComEmitterDireation = CreateDefaultSubobject<UArrowComponent>("HPlaComEmitterDireation");
	ComEmitterDireation->SetupAttachment(ComEmitterLocation);
	ComEmitterDireation->SetHiddenInGame(false);
	ComEmitterDireation->SetRelativeTransform(FTransform::Identity);



	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	ComEmitterMoveController->SetSphereRadius(EmitterMoveRadiance, true);
	ComEmitterMoveController->SetRelativeLocation(EmitterMoveOffSet);

}

void AHPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ComEmitterLocation->SetRelativeLocation(FVector(EmitterMoveRadiance, 0.f, 0.f));

}

// Called to bind functionality to input
void AHPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AHPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AHPlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("ChangeCore", this, &AHPlayerCharacter::ChangeCore);

	//AddControllerYawInput at Apawn
	//you should open "Use Pawn Control Rotation" at springArmCom to allow playerController to controll arm
	PlayerInputComponent->BindAxis("MouseX", this, &AHPlayerCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("MouseY", this, &AHPlayerCharacter::AddControllerPitchInput);
	
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed,this, &AHPlayerCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Released,this, &AHPlayerCharacter::StopJump);
	
	PlayerInputComponent->BindAction("Crouch", EInputEvent::IE_Pressed,this, &AHPlayerCharacter::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", EInputEvent::IE_Released,this, &AHPlayerCharacter::StopCrouch);
	
	PlayerInputComponent->BindAction("PrimaryAttack",IE_Pressed,this,&AHPlayerCharacter::OnTriggerPressed);
	PlayerInputComponent->BindAction("PrimaryAttack",IE_Released,this,&AHPlayerCharacter::OnTriggerReleased);
	
	PlayerInputComponent->BindAction("RestoreEnergy",IE_Pressed,this,&AHPlayerCharacter::OnStartRestoreEnergy);
	PlayerInputComponent->BindAction("RestoreEnergy",IE_Released,this,&AHPlayerCharacter::OnStopedRestoreEnergy);
}

void AHPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector EmitterLocation = ComEmitterMoveController->GetRelativeLocation() + GetActorLocation() + GetControlRotation().Vector() * EmitterMoveRadiance ;
	FRotator EmitterRotation = (GetControlRotation() + EmitterDirectionOffSet).GetNormalized();
	ComEmitterLocation->SetWorldLocation(EmitterLocation);
	ComEmitterLocation->SetWorldRotation(EmitterRotation);

}



void AHPlayerCharacter::OnStopedRestoreEnergy()
{
	ComActions->StopActionByName(this, "RestoreEnergy");
}

void AHPlayerCharacter::OnStartRestoreEnergy()
{
	ComActions->StartActionByName(this, "RestoreEnergy");

}


void AHPlayerCharacter::StartJump()
{
	Super::Jump();
	bJumpButtonDown = CanJump();
	DispatchJumpData();

}

void AHPlayerCharacter::StopJump()
{
	Super::StopJumping();
	bJumpButtonDown = false;
	DispatchJumpData();
}

void AHPlayerCharacter::DispatchJumpData()
{
	UCharacterMovementComponent* ComCM = GetCharacterMovement();
	if (ComCM)
	{
		ComCM->JumpZVelocity = 400.f;
	}
}

void AHPlayerCharacter::OnTriggerPressed()
{
	ComActions->StartActionByName(this, TEXT("TriggerEmitter"));
}

void AHPlayerCharacter::OnTriggerReleased()
{
	ComActions->StopActionByName(this, TEXT("TriggerEmitter"));

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

void AHPlayerCharacter::MoveForward(float val)
{
	FRotator controlRot = GetControlRotation();
	controlRot.Roll = 0.f;
	controlRot.Pitch = 0.f;
	// commit rotateVector to player comtroller;
	AddMovementInput(controlRot.Vector(), val);
}

void AHPlayerCharacter::ChangeCore(float val)
{
	if (val)
		ComEmitter->SlideCore(val);

}
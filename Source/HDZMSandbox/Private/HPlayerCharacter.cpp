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

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;


}




// Called when the game starts or when spawned
void AHPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHPlayerCharacter::OnRestoreEnergy()
{

	// make RestoreEnergy invalid / break this one Restore by set it 0 ;
	if (RestoreEnergyKeyPressedTick == 0)
		return;

	//TicksPerSecond 10000000
	float holdSeconds = (FDateTime::Now().GetTicks() - RestoreEnergyKeyPressedTick )/ 10000000.f;
	// @fix: may need a little UI Animate

	UE_LOG(LogTemp, Warning, TEXT("OnRestoreEnergy holdSeconds:%f"), holdSeconds);

	if (holdSeconds > 2.5f)
		ComAttribute->RestoreEnergyToInitLevel();

}


void AHPlayerCharacter::OnStartRestoreEnergy()
{
	LogOnScreen(this, "OnStartRestoreEnergy");

	RestoreEnergyKeyPressedTick = FDateTime::Now().GetTicks();
}

void AHPlayerCharacter::ChangeCore(float val)
{
	if(val)
		ComEmitter->SlideCore(val);

}

void AHPlayerCharacter::PrimaryEmitt()
{
	ComActions->StartActionByName(this,TEXT("TriggerEmitter"));

	RestoreEnergyKeyPressedTick = 0;
}

void AHPlayerCharacter::MoveRight(float val)
{
	FRotator controlRot = GetControlRotation();
	controlRot.Roll = 0.f;
	controlRot.Pitch = 0.f;

	//X means forward then Y means right;
	FVector rightVector = FRotationMatrix(controlRot).GetScaledAxis(EAxis::Y);
	AddMovementInput(rightVector, val);

	if(val)
		RestoreEnergyKeyPressedTick = 0;

}


void AHPlayerCharacter::MoveForward(float val)
{
	FRotator controlRot = GetControlRotation();
	controlRot.Roll = 0.f;
	controlRot.Pitch = 0.f;
	// commit rotateVector to player comtroller;
	AddMovementInput(controlRot.Vector(), val);

	if (val)
		RestoreEnergyKeyPressedTick = 0;
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
	PlayerInputComponent->BindAxis("ChangeCore", this, &AHPlayerCharacter::ChangeCore);

	//AddControllerYawInput at Apawn
	//you should open "Use Pawn Control Rotation" at springArmCom to allow playerController to controll arm
	PlayerInputComponent->BindAxis("MouseX", this, &AHPlayerCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("MouseY", this, &AHPlayerCharacter::AddControllerPitchInput);
	
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed,this, &AHPlayerCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Released,this, &AHPlayerCharacter::StopJump);
	
	PlayerInputComponent->BindAction("Crouch", EInputEvent::IE_Pressed,this, &AHPlayerCharacter::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", EInputEvent::IE_Released,this, &AHPlayerCharacter::StopCrouch);
	
	PlayerInputComponent->BindAction("PrimaryAttack",IE_Pressed,this,&AHPlayerCharacter::PrimaryEmitt);
	
	PlayerInputComponent->BindAction("RestoreEnergy",IE_Pressed,this,&AHPlayerCharacter::OnStartRestoreEnergy);
	PlayerInputComponent->BindAction("RestoreEnergy",IE_Repeat,this,&AHPlayerCharacter::OnRestoreEnergy);
	


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
	RestoreEnergyKeyPressedTick = 0;
}

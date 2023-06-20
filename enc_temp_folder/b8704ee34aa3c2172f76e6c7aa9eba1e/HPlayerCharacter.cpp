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


	ComAlsCamera = CreateDefaultSubobject<UAlsCameraComponent>(TEXT("AlsCamera"));
	ComAlsCamera->SetupAttachment(GetMesh());
	ComAlsCamera->SetRelativeRotation_Direct({ 0.0f, 90.0f, 0.0f });

	//test and set in BP
	//@check UCharacterMovementComponent .h 


	ComActions = CreateDefaultSubobject<UHActionComponent>("HPlaComAction");
	ComAttribute = CreateDefaultSubobject<UHAttributeComponent>("HPlaComAttribute");


	ComSphereCollision = CreateDefaultSubobject<USphereComponent>("HPlaComSphereCollision");
	ComSphereCollision->SetupAttachment(GetRootComponent());



	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;


}

void AHPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();


}

void AHPlayerCharacter::CalcCamera(const float DeltaTiem, FMinimalViewInfo& ViewInfo)
{
	if (!ComAlsCamera->IsActive())
	{
		Super::CalcCamera(DeltaTiem,ViewInfo);
		return;
	}
	ComAlsCamera->GetViewInfo(ViewInfo);
}


// Called to bind functionality to input
void AHPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AHPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AHPlayerCharacter::MoveRight);

	//AddControllerYawInput at Apawn
	//you should open "Use Pawn Control Rotation" at springArmCom to allow playerController to controll arm
	PlayerInputComponent->BindAxis(TEXT("LookRightMouse"), this, &AHPlayerCharacter::OnLookUp);
	PlayerInputComponent->BindAxis(TEXT("LookUpMouse"), this, &AHPlayerCharacter::OnLookRight);
	
	PlayerInputComponent->BindAction("Trigger",IE_Pressed,this,&AHPlayerCharacter::OnTriggerPressed);
	PlayerInputComponent->BindAction("Trigger",IE_Released,this,&AHPlayerCharacter::OnTriggerReleased);
	
	PlayerInputComponent->BindAction("RestoreEnergy",IE_Pressed,this,&AHPlayerCharacter::OnStartRestoreEnergy);
	PlayerInputComponent->BindAction("RestoreEnergy",IE_Released,this,&AHPlayerCharacter::OnStopedRestoreEnergy);

	PlayerInputComponent->BindAction(TEXT("Aim"), IE_Pressed, this, &ThisClass::OnAimPressed);
	PlayerInputComponent->BindAction(TEXT("Aim"), IE_Released, this, &ThisClass::OnAimReleased);


	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &ThisClass::OnSprintPressed);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &ThisClass::OnSprintReleased);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_DoubleClick, this, &ThisClass::OnRoll);

	PlayerInputComponent->BindAction(TEXT("Walk"), IE_Pressed, this, &ThisClass::OnWalk);
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &ThisClass::OnCrouch);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ThisClass::OnJumpPressed);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &ThisClass::OnJumpReleased);

}

void AHPlayerCharacter::OnAimPressed()
{
	SetDesiredAiming(true);
}

void AHPlayerCharacter::OnAimReleased()
{
	SetDesiredAiming(false);
}

void AHPlayerCharacter::OnLookUp(float val)
{
	AddControllerPitchInput(val);
}

void AHPlayerCharacter::OnLookRight(float val)
{

	AddControllerYawInput(val);
}


void AHPlayerCharacter::OnWalk()
{
	if (GetWorldTimerManager().TimerExists(SprintStartTimer))
	{
		return;
	}

	// ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
	// ReSharper disable once CppIncompleteSwitchStatement
	switch (GetDesiredGait())
	{
	case EAlsGait::Walking:
		SetDesiredGait(EAlsGait::Running);
		break;

	case EAlsGait::Running:
		SetDesiredGait(EAlsGait::Walking);
		break;
	}
}

void AHPlayerCharacter::OnCrouch()
{
	switch (GetDesiredStance())
	{
	case EAlsStance::Standing:
		SetDesiredStance(EAlsStance::Crouching);
		break;

	case EAlsStance::Crouching:
		SetDesiredStance(EAlsStance::Standing);
		break;
	}
}

void AHPlayerCharacter::OnJumpPressed()
{
	if (TryStopRagdolling())
	{
		return;
	}

	if (TryStartMantlingGrounded())
	{
		return;
	}

	if (GetStance() == EAlsStance::Crouching)
	{
		SetDesiredStance(EAlsStance::Standing);
		return;
	}

	Jump();
}

void AHPlayerCharacter::OnJumpReleased()
{
	StopJumping();
}

void AHPlayerCharacter::OnSprintPressed()
{
	// Start the sprint with a slight delay to give the player enough time to start the roll with a double click instead.

	static constexpr auto StartDelay{ 0.1f };

	GetWorldTimerManager().SetTimer(SprintStartTimer,
		FTimerDelegate::CreateWeakLambda(this, [this]
			{
				SetDesiredGait(EAlsGait::Sprinting);
			}), StartDelay, false);
}

void AHPlayerCharacter::OnSprintReleased()
{
	if (GetWorldTimerManager().TimerExists(SprintStartTimer))
	{
		GetWorldTimerManager().ClearTimer(SprintStartTimer);
	}

	else
	{
		SetDesiredGait(EAlsGait::Running);
	}
}

void AHPlayerCharacter::OnRoll()
{
	GetWorldTimerManager().ClearTimer(SprintStartTimer);
	static constexpr auto PlayRate{ 1.3f };
	TryStartRolling(PlayRate);

}

void AHPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void AHPlayerCharacter::OnStopedRestoreEnergy()
{
	ComActions->StopActionByName(this, "RestoreEnergy");
}

void AHPlayerCharacter::OnStartRestoreEnergy()
{
	ComActions->StartActionByName(this, "RestoreEnergy");

}


void AHPlayerCharacter::OnTriggerPressed()
{
	if ( GetDesiredRotationMode() == EAlsRotationMode::Aiming)
	{
		ShootType = EHShootType::BackwardShoot;
	}
	else ShootType = EHShootType::ForwardShoot;

	ComActions->StartActionByName(this, TEXT("TriggerEmitter"));
}

void AHPlayerCharacter::OnTriggerReleased()
{

	ShootType = EHShootType::None;
	
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

void AHPlayerCharacter::DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& Unused, float& VerticalPosition)
{
	if (ComAlsCamera->IsActive())
	{
		ComAlsCamera->DisplayDebug(Canvas, DebugDisplay, VerticalPosition);
	}

	Super::DisplayDebug(Canvas, DebugDisplay, Unused, VerticalPosition);
}

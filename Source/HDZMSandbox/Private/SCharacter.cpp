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
#include "SActionComponent.h"
#include "../HDZMSandbox.h"
#include "GameFramework/Controller.h"
#include "SPlayerState.h"
#include "Components/CapsuleComponent.h"
#include "Components/PrimitiveComponent.h"




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

	ComSInteraction = CreateDefaultSubobject<USInteractionComponent>("SChaComSInteraction");

	ComActions = CreateDefaultSubobject<USActionComponent>("SChaComAction");


	//make char rotate CharmainDirection  to movement direction
	GetCharacterMovement()->bOrientRotationToMovement = true; 

	//it used to be set true in default;
	bUseControllerRotationYaw = false; 

	//DashDistance = 3.f;

	HitTimeParametersName = TEXT("HitTime");

}

FVector ASCharacter::GetPawnViewLocation() const
{
	return ComCamera->GetComponentLocation();
}

void ASCharacter::OnHealthChanged(USAttributeComponent* owningComp, AActor* instigatorActor, float newHealth, float delta)
{
	APlayerController* plyarCont =Cast<APlayerController>(GetController());
	if (IsLocallyControlled())
		plyarCont->ClientPlayCameraShake(CamerShakeDamaged);


	if (delta < 0.f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(HitTimeParametersName, GetWorld()->GetTimeSeconds());
		
		//damage cause rage
		float RageDelta = FMath::Abs(delta);
		ComSAttribute->ApplyRageChangeDelta(RageDelta,instigatorActor );

		//if die , can't control
		if (newHealth <= 0.f )
		{
			APlayerController* controllerPC =Cast<APlayerController>(GetController());
			//close old collision
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();


			//disapeare
			SetLifeSpan(10.f);
			
			DisableInput(controllerPC);
		}
		// Rage added equal to damage received (Abs to turn into positive rage number)
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
	ComActions->StartActionByName(this,"Dash");
}

void ASCharacter::SprintStart()
{
	ComActions->StartActionByName(this,"Sprint");
}

void ASCharacter::SprintStop()
{
	ComActions->StopActionByName(this,"Sprint");
}

void ASCharacter::PrimaryAttack()
{

	ComActions->StartActionByName(this, "PrimaryAttack");

}

void ASCharacter::BlackholeAttack()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, "BlackholeAttack: Part On Character");

	ComActions->StartActionByName(this, "BlackHole");
}



// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ASPlayerState* ps = Cast<ASPlayerState>(GetPlayerState());
	if (ps)
	{
		FString msg  =  FString::Printf(TEXT("Player %s credits: %d"),*GetNameSafe(this),ps->GetCredit());
		LogOnScreen(this, msg, FColor::Blue, 0.f);
	}

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);


	//AddControllerYawInput at Apawn
	//you should open "Use Pawn Control Rotation" at springArmCom to allow playerController to controll arm
	PlayerInputComponent->BindAxis("LookRightMouse", this, &ASCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUpMouse", this, &ASCharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack",IE_Pressed,this,&ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&ASCharacter::Jump);
	
	//you cant directly call componentMethod;
	PlayerInputComponent->BindAction("PrimaryInteract",IE_Pressed, this, &ASCharacter::PrimaryInteraction);
	//bind E
	PlayerInputComponent->BindAction("BlackholeAttack",IE_Pressed, this, &ASCharacter::BlackholeAttack);

	PlayerInputComponent->BindAction("Dash",IE_Pressed, this, &ASCharacter::Dash);
	
	PlayerInputComponent->BindAction("Sprint",IE_Pressed, this, &ASCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint",IE_Released, this, &ASCharacter::SprintStop);

	
}

void ASCharacter::HealSelf(float amount /* = 100*/)
{
	ComSAttribute->ApplyHealthChangeDelta(amount,this);
}


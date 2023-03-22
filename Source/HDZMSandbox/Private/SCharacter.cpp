// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//foue chr of class name + Varname
	ComSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SChaComSprinArm"));
	ComSpringArm->SetupAttachment(RootComponent);

	ComSpringArm->TargetArmLength = 350.f;

	ComCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("SChaComCamera"));
	ComCamera->SetupAttachment(ComSpringArm);

	

}

void ASCharacter::MoveForward(float Val)
{
	// commit to player comtroller;
	AddMovementInput(GetActorForwardVector(), Val);
}

void ASCharacter::MoveRight(float Val)
{
	AddMovementInput(GetActorRightVector(), Val);
		
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
	
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ASCharacter::MoveRight);


	//AddControllerYawInput at Apawn
	PlayerInputComponent->BindAxis(TEXT("MouseX"), this, &ASCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("MouseY"), this, &ASCharacter::AddControllerPitchInput);




}




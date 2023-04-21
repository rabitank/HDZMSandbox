// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerController.h"
#include "Blueprint/UserWidget.h"

void ASPlayerController::TogglePauseMenu()
{
	if (PauseMeanuInstance && PauseMeanuInstance->IsInViewport())
	{

		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());

		PauseMeanuInstance->RemoveFromParent();
		PauseMeanuInstance = nullptr;
		return;
	}

	PauseMeanuInstance = CreateWidget<UUserWidget>(this,PauseMeanuClass);
	if (PauseMeanuInstance)
	{
		PauseMeanuInstance->AddToViewport();
		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
	}

}

void ASPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("PauseGame",EInputEvent::IE_Pressed,this,&ASPlayerController::TogglePauseMenu);

}

void ASPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	OnPawnChanged.Broadcast(InPawn);
}

void ASPlayerController::BeginPlayingState()
{
	BluePrintBeginPlayingState();
}

void ASPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	OnRecivedPlayerState.Broadcast(PlayerState);
}

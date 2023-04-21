// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Delegates/DelegateCombinations.h"
#include "SPlayerController.generated.h"

class APlayerState;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnChanged, APawn*,newPawn);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRecivedPlayerState, APlayerState*,playState);


/**
 * 
 */
UCLASS()
class HDZMSANDBOX_API ASPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PauseMeanuClass;

	UPROPERTY()
	class UUserWidget* PauseMeanuInstance;

	UPROPERTY(BlueprintAssignable)
	FOnPawnChanged OnPawnChanged;

	//Toggle:切换
	UFUNCTION(BlueprintCallable)
		void TogglePauseMenu();

	virtual void SetupInputComponent() override;

	UPROPERTY(BlueprintAssignable)
		FOnRecivedPlayerState OnRecivedPlayerState;

	UFUNCTION()
		virtual void SetPawn(APawn* InPawn) override;
	
	/** Pawn has been possessed, so changing state to NAME_Playing. Start it walking and begin playing with it. */
	virtual void BeginPlayingState() override;


	UFUNCTION(BlueprintImplementableEvent)
		void BluePrintBeginPlayingState();

	void OnRep_PlayerState() override ;

};

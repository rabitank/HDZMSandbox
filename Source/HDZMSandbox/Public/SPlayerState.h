// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Delegates/DelegateCombinations.h"
#include "SPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged,ASPlayerState*,PlayerState,int32,NewCredits,int32,Delta);

/**
 * 
 */
UCLASS()
class HDZMSANDBOX_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()
protected:

	UPROPERTY(EditAnywhere,Replicated,Category = "Credit")
	int32 Credit;

public:
	ASPlayerState();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastOnCreditsChanged(ASPlayerState* ps, int32 nweCredit, int32 delta);

	UFUNCTION(BlueprintCallable,Category="Credit")
	void AddCredit(int32 delta);

	UFUNCTION(BlueprintCallable,Category="Credit")
	bool RemoveCredit(int32 delta);
	
	UFUNCTION(BlueprintCallable,Category="Credit")
	int32 GetCredit() const;

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
		void SavePlayerState(class USSaveGame* currentSave);
	UFUNCTION()
		void LoadPlayerState(class USSaveGame* currentSave);


	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCreditsChanged OnCreditsChanged ;



	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class HDZMSANDBOX_API AHGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);
};

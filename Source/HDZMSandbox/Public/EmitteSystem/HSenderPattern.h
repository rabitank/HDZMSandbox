// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HSenderPattern.generated.h"

UCLASS()
class HDZMSANDBOX_API AHSenderPattern : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHSenderPattern();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TArray<TSubclassOf<class AHSender>>  Senders;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TArray<AHSender*>  SendersIns;

public:	

	virtual void InitPattern();

};

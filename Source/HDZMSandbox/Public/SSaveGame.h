// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SSaveGame.generated.h"

USTRUCT()
struct FActorData
{
	GENERATED_BODY()
public:
	UPROPERTY()
		FString ActorName;
	
	UPROPERTY()
	FTransform ActorTransform;
	
	UPROPERTY()
	TArray<uint8> ByteData;

};

/**
 * 
 */
UCLASS()
class HDZMSANDBOX_API USSaveGame : public USaveGame
{
	GENERATED_BODY()
public:

	UPROPERTY()
	int32 Credit;
	
	UPROPERTY()
	TArray<FActorData> SaveActorDatas;


};

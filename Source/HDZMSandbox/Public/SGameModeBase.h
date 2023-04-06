// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "SGameModeBase.generated.h"


UCLASS()
class HDZMSANDBOX_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:

	FTimerHandle TimerHandle_BotSpawn;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UEnvQuery* SpawnBotQuery;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<class AActor> MinionClass;




	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UCurveFloat* DifficultyCurve;
	//难度曲线

	UFUNCTION()
	void OnQueryCompleted(class UEnvQueryInstanceBlueprintWrapper* queryInstance , EEnvQueryStatus::Type  queryStatus);

	UFUNCTION()
	void SpawnBotTimerElapsed();

public:
	ASGameModeBase();

	virtual void StartPlay() override;

};

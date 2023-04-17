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

	UPROPERTY(EditDefaultsOnly, Category = "PlayerState | Credits")
		int32 CreditsPerKilled;
	UPROPERTY(EditDefaultsOnly, Category = "PowerUp")
		int32 DesiredPowerUpCount;
	UPROPERTY(EditDefaultsOnly, Category = "PowerUp")
		float RequiredPowerUpDistance;
	
	UPROPERTY(EditDefaultsOnly, Category = "PowerUp")
	TArray<TSubclassOf<class ASPowerUpActor>>PowerupClasses;
	
	UPROPERTY(EditDefaultsOnly, Category = "PowerUp")
	class UEnvQuery* SpawnPowerUpQuery;


	

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
		void OnQueryCompleted(class UEnvQueryInstanceBlueprintWrapper* queryInstance, EEnvQueryStatus::Type  queryStatus);
	UFUNCTION()
		void OnPowerUpSpawnQueryCompleted(class UEnvQueryInstanceBlueprintWrapper* queryInstance, EEnvQueryStatus::Type queryStatus);


	UFUNCTION()
		void SpawnBotTimerElapsed();

	void SpawnPowerUpQueryImp();
	
public:
	ASGameModeBase();

	

	UFUNCTION(Exec)
		void KillAll();
	
	UFUNCTION(Exec)
		void ClearCredit();

	virtual void StartPlay() override;

	UFUNCTION()
		void RespawnPlayerDelayElaps(AController* controller);

	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);


};

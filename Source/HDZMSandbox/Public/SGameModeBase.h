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

	FString SlotName;

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
	
	
	UPROPERTY(EditDefaultsOnly, Category = "SaveGame")
	class USSaveGame* CurrentSaveGame;

	
	

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


	/**
	 * Initialize the game.
	 * The GameMode's InitGame() event is called before any other functions (including PreInitializeComponents() )
	 * and is used by the GameMode to initialize parameters and spawn its helper classes.
	 * @warning: this is called before actors' PreInitializeComponents.
	 */
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	

	/** Signals that a player is ready to enter the game, which may start it up */
	//UFUNCTION(BlueprintNativeEvent, Category = Game)
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	UFUNCTION(BlueprintCallable)
		void WriteSaveGame();
	UFUNCTION()
		void LoadSaveGame();

	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);


};

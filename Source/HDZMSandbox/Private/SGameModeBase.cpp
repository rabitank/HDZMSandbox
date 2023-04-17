// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/Actor.h"
#include "EngineUtils.h"
#include "AI/SAICharacter.h"
#include "SAttributeComponent.h"
#include "DrawDebugHelpers.h"
#include "SCharacter.h"
#include "SPlayerState.h"
#include "SPowerUpActor.h"
#include "GameFramework/GameSession.h"


//控制台变量
static TAutoConsoleVariable<bool> CvarSpawnBots(TEXT("su.SpawnBots"), false, TEXT("Enable spawning of bots via timer."),ECVF_Cheat);
//ECVF_Cheat: when package game ,this ConsoleVariable will be ignored;

ASGameModeBase::ASGameModeBase()
{
	
	SpawnTimerInterval = 2.f;
	CreditsPerKilled = 20.f;

	DesiredPowerUpCount = 20;
	RequiredPowerUpDistance = 2000.f;

	PlayerStateClass = ASPlayerState::StaticClass();

	bReplicates = true;

}


void ASGameModeBase::KillAll()
{
	for (TActorIterator<ASAICharacter> it(GetWorld()); it; ++it)
	{
		ASAICharacter* bot = *it;
		USAttributeComponent* comAttribute = USAttributeComponent::GetAttributes(bot);
		if (comAttribute && comAttribute->IsAlive())
		{
			comAttribute->Kill(this); //should pass player?
		}
	}
}



void ASGameModeBase::SpawnBotTimerElapsed()
{
	if (!CvarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("bot spawning Disabled via var 'CvarSpawnBots' "));
		return;
	}

	 UEnvQueryInstanceBlueprintWrapper* queryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery,this,EEnvQueryRunMode::RandomBest5Pct,nullptr);
	 //5Pct, random choose one in best 5?

	 if (ensure(queryInstance))
	 {
		 queryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryCompleted);
	 }
}

void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* queryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnBot EQS query failed"));
		return;
	}

	int32 NrofAliveBots = 0;

	//TActorIterator<ASAICharacter> it(GetWorld()) iterat all ASAICharacter in world
	//it may be very useful
	for (TActorIterator<ASAICharacter> it(GetWorld()); it; ++it)
	{
		ASAICharacter* bot = *it;
		USAttributeComponent* comAttribute =USAttributeComponent::GetAttributes(bot);
		if (comAttribute && comAttribute->IsAlive())
		{
			NrofAliveBots+=1;
		}
	}

	float curMaxNum=0.f ;
	if (DifficultyCurve)
	{
		curMaxNum = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
		//read maxBotNum , seconds as x axis;
	}
	if (NrofAliveBots >= curMaxNum)
	{
		UE_LOG(LogTemp, Warning, TEXT("cur Bot num has reached the curmaxnum,skip spawn"));
		return;
	}

	 

	TArray<FVector> locations = queryInstance->GetResultsAsLocations(); //a array of sorted location?



	if(locations.Num()>0)
	{
		GetWorld()->SpawnActor<AActor>(MinionClass,locations[0],FRotator::ZeroRotator);
		DrawDebugSphere(GetWorld(),locations[0],50.f,16,FColor::White,false,100.f);

	}

}
/// <summary>
/// @Describe: for locations by EQS, random choose locations keep RequiredPowerUpDistance spawn DesiredPowerUpCount random powerupActor
/// </summary>
/// <param name="queryInstance"> PowerUpSpawnLocation EQS </param>
/// <param name="queryStatus"></param>
void ASGameModeBase::OnPowerUpSpawnQueryCompleted(class UEnvQueryInstanceBlueprintWrapper* queryInstance, EEnvQueryStatus::Type queryStatus)
{
	if (queryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp,Warning,TEXT("Spawn Powerup potion EQS query Failed!"))
		return;
	}

	TArray<FVector> locations= queryInstance->GetResultsAsLocations();
	TArray<FVector> UsedLocations;
	int32 SpawnCounter = 0;

	while (SpawnCounter<DesiredPowerUpCount && locations.Num()>0 )
	{
		int32 RandomLocaitonIndex = FMath::RandRange(0, locations.Num() - 1);

		FVector PickedLocation = locations[RandomLocaitonIndex];
		
		locations.RemoveAt(RandomLocaitonIndex);
		bool bValidLocation = true;
		
		for(FVector otherlocation : UsedLocations)
		{
			float DistanceTo = (PickedLocation - otherlocation).Size();
			if (DistanceTo < RequiredPowerUpDistance)
			{
				bValidLocation= false;
				break;
			}
		}

		if(!bValidLocation)
			continue;

		int32 RandomClassIndex = FMath::RandRange(0, PowerupClasses.Num() - 1);
		TSubclassOf<AActor> powerupActor = PowerupClasses[RandomClassIndex];
		GetWorld()->SpawnActor<AActor>(powerupActor, PickedLocation,FRotator::ZeroRotator);

		UsedLocations.Add(PickedLocation);
		SpawnCounter++;

	}

}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	//loop query for location of spawning bot
	GetWorldTimerManager().SetTimer(TimerHandle_BotSpawn,this,&ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval,true);
	SpawnPowerUpQueryImp();

}
void ASGameModeBase::SpawnPowerUpQueryImp()
{
	UEnvQueryInstanceBlueprintWrapper* queryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnPowerUpQuery, this, EEnvQueryRunMode::AllMatching, nullptr);
	if (ensure(queryInstance))
	{
		queryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnPowerUpSpawnQueryCompleted);
	}
}


void ASGameModeBase::RespawnPlayerDelayElaps(AController* controller)
{
	if (ensure(controller))
	{
		controller->UnPossess(); // controller don't possess old pawn

		RestartPlayer(controller);
	}
}

void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	//if player been killed

	ASCharacter* player = Cast<ASCharacter>(VictimActor);
	if (player)
	{
		FTimerHandle TimerHandle_RespawnPlayerDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerDelayElaps", player->GetController());

		float RespawnDelay = 2.f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnPlayerDelay, Delegate, RespawnDelay, false);


		
	}

	APawn* killerPawn = Cast<APawn>(Killer);
	if (killerPawn)
	{
		ASPlayerState* ps = Cast<ASPlayerState>(killerPawn->GetPlayerState());
		if (ps)
		{
			ps->AddCredit(CreditsPerKilled);
		}
	}

}

void ASGameModeBase::ClearCredit()
{

	ASPlayerState* ps = Cast<ASPlayerState>(GetWorld()->GetFirstPlayerController()->GetPawn()->GetPlayerState());
	ps->RemoveCredit(ps->GetCredit());

}

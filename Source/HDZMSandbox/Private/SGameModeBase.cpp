// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/Actor.h"
#include "EngineUtils.h"
#include "AI/SAICharacter.h"
#include "SAttributeComponent.h"
#include "DrawDebugHelpers.h"

ASGameModeBase::ASGameModeBase()
{

	SpawnTimerInterval = 2.f;
}


void ASGameModeBase::SpawnBotTimerElapsed()
{
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

	float curMaxNum ;
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

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	//loop query for location of spawning bot
	GetWorldTimerManager().SetTimer(TimerHandle_BotSpawn,this,&ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval,true);
}

// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

//LogOnScreen with NetPrefix
static void LogOnScreen(UObject* WorldContext, FString Msg, FColor Color = FColor::White, float Duration = 5.f)
{
	if (!ensure(WorldContext))
	{
		return;
	}

	UWorld* world = WorldContext->GetWorld();
	if (!ensure(world))
	{
		return;
	}

	FString NetPrefix = world->IsNetMode(NM_Client) ? "[Client]" : "[Server]";
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, Duration, Color, NetPrefix + Msg);
	}

}
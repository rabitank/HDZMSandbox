// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "HEmitterSettings.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class HDZMSANDBOX_API UHEmitterSettings : public UDataAsset
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "RawConfig| Bullet Recoil")
		TMap<FName,float > PatternRecoilMap;
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "RawConfig| Bullet Recoil")
		TMap<FName,float > PatternBaseEnergy;

	float GetPatternRecoil(const FName& emittePatternName) const { return PatternRecoilMap.FindRef(emittePatternName); };
	float GetPatternBaseEnergy (const FName& emittePatternName) const { return PatternBaseEnergy.FindRef(emittePatternName); };

};

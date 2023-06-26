// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "HBulletBase.h"
#include "HSenderSettings.generated.h"

/**
 * 
 */



UCLASS(Blueprintable)
class HDZMSANDBOX_API UHSenderSettings : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseCurve{ true };
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FVector SenderInitPosition {0.f,0.f,0.f};
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FRotator SenderInitRotator {0.f,0.f,0.f};
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float  SenderAngularVelocaity{0.f};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float Interval{0.2f};
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FRotator> BulletTraces;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int Counts{0};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSubclassOf<AHBulletBase> BulletType;

	//可能会有行为更复杂的子弹(追踪弹. 这里就不提供Bullet数据了
	//话说bullet真的能被数据驱动吗?
	
	UHSenderSettings();


	
};

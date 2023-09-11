// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LineSTG/HBulletBase.h"
#include "HSenderPattern.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FEmittePatternFullShootDelegate, AHSenderPattern*, senderPatternIns, AHBulletBase*, YX, AActor*, senderOwner);


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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AHBulletBase* SampleBullet;

	unsigned int CurSender{0};
	unsigned int CurBulletNums{0};
	
public:	

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void ResetPatternShoot(float interval,AHBulletBase* samplebulle);
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void InitPattern(float interval,AHBulletBase* samplebulle);
	
	//return a Sender on list
	UFUNCTION(BlueprintCallable)
	AHSender* Pop();

	
	void OnSampleBulletClone(AHBulletBase* YX, FName bulletName, AHBulletBase* cloneIns);
	
	UPROPERTY(BlueprintAssignable)
		FEmittePatternFullShootDelegate OnFullShootDelegate;
};

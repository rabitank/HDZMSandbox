// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LineSTG/HBulletBase.h"
#include "HSenderPattern.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FEmittePatternFullShootDelegate, class AHSenderPattern*, senderPatternIns, AHBulletBase*, YX, AActor*, senderOwner);

USTRUCT(BlueprintType)
struct FSenderInitData
	// ReSharper disable once UnrealHeaderToolError
{
	GENERATED_BODY()

	FSenderInitData() = default;
	
	FSenderInitData(FVector pos,FRotator rot,int index):
	senderInitPos(pos),senderInitAbsRot(rot),senderClassIndex(index){}

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector senderInitPos{0.f,0.f,0.f};
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FRotator senderInitAbsRot{0.f,0.f,0.f};
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int senderClassIndex{0};
	
	
};

UCLASS()
class HDZMSANDBOX_API AHSenderPattern : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHSenderPattern();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TArray<TSubclassOf<class AHSender>>  SenderClasses;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TArray<AHSender*>  SendersIns;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AHBulletBase* SampleBullet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FSenderInitData> SendersInitDatas;
	
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

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void SetSendersInitData( TArray<FSenderInitData>& datas);

	UFUNCTION()
	void OnSampleBulletClone(AHBulletBase* YX, FName bulletName, AHBulletBase* cloneIns);

	UFUNCTION(BlueprintCallable)
	void OpenSenders();

	UFUNCTION(BlueprintCallable)
	void CloseSenders();
	
	UPROPERTY(BlueprintAssignable)
		FEmittePatternFullShootDelegate OnFullShootDelegate;
};

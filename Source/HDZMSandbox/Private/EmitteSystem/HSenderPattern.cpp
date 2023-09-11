// Fill out your copyright notice in the Description page of Project Settings.


#include "EmitteSystem/HSenderPattern.h"
#include "EmitteSystem/HSender.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHSenderPattern::AHSenderPattern()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AHSenderPattern::ResetPatternShoot_Implementation(float interval, AHBulletBase* samplebullet)
{
	if(IsValid(samplebullet))
	{
		SampleBullet->OnBulletClone.RemoveDynamic(this,&AHSenderPattern::OnSampleBulletClone);
		SampleBullet = samplebullet;
		samplebullet->OnBulletClone.AddDynamic(this,&AHSenderPattern::OnSampleBulletClone);
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("ResetPattern Failed:%s's samplerbullet isn't valid"),*GetName());
		return;
	}
	for (AHSender* senderC : SendersIns)
	{
		senderC->ResetInterval(interval);
		senderC->ResetSampleBullet(samplebullet);
	}
}

void AHSenderPattern::InitPattern_Implementation(float interval, AHBulletBase* samplebullet)
{
	if(IsValid(samplebullet)) SampleBullet = samplebullet;
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("InitPattern Failed:%s's samplerbullet isn't valid"),*GetName());
		return;
	}
	for (TSubclassOf<AHSender> senderC : Senders)
	{
		//TODO: SetHEmitterPatterns Insitgator as Player;And other set;
		AHSender* senderIns =  Cast<AHSender>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this,senderC,GetTransform(),ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
		ensure(senderIns);
		senderIns->InitSender_IS(interval,samplebullet);
		senderIns->SetOwner(this);
		senderIns->SetInstigator(GetInstigator());
		UGameplayStatics::FinishSpawningActor(senderIns,GetTransform());
		senderIns->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		SendersIns.Add(senderIns);

		samplebullet->OnBulletClone.AddDynamic(this,&AHSenderPattern::OnSampleBulletClone);
	}
}

AHSender* AHSenderPattern::Pop()
{
	if (CurSender==SendersIns.Num())
	{
		CurSender=0;
	}
	return SendersIns[CurSender++];
	
}

void AHSenderPattern::OnSampleBulletClone(AHBulletBase* YX, FName bulletName, AHBulletBase* cloneIns)
{
	CurBulletNums++;
	if(CurBulletNums%SendersIns.Num()==0)
	{
		OnFullShootDelegate.Broadcast(this,SampleBullet,GetOwner());
	}
	
}





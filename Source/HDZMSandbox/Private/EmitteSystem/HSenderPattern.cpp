// Fill out your copyright notice in the Description page of Project Settings.


#include "EmitteSystem/HSenderPattern.h"
#include "EmitteSystem/HSender.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

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
	ensure(SenderClasses[0]);
	SetSendersInitData(SendersInitDatas);
	
	ensure(SendersInitDatas.IsValidIndex(0));
	if(ensure(samplebullet)) SampleBullet = samplebullet;
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("InitPattern Failed:%s's samplerbullet isn't valid"),*GetName());
		return;
	}
	samplebullet->OnBulletClone.AddDynamic(this,&AHSenderPattern::OnSampleBulletClone);

	const FAttachmentTransformRules rule{EAttachmentRule::KeepWorld,EAttachmentRule::KeepWorld,EAttachmentRule::KeepWorld,false};
	for (const FSenderInitData& SenderD : SendersInitDatas)
	{
		const FTransform transform(SenderD.senderInitAbsRot,
			    SenderD.senderInitPos.operator+(GetActorLocation()));
		 
		//TODO: SetHEmitterPatterns Insitgator as Player;And other set;
		AHSender* senderIns =  Cast<AHSender>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this,
			SenderClasses.IsValidIndex(SenderD.senderClassIndex)?SenderClasses[SenderD.senderClassIndex]:SenderClasses.Last(),
			transform,ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
		ensure(senderIns);
		senderIns->InitSender_IS(interval,samplebullet);
		senderIns->SetOwner(this);
		senderIns->SetInstigator(GetInstigator());
		UGameplayStatics::FinishSpawningActor(senderIns,transform);

		senderIns->AttachToActor(this,rule);
		SendersIns.Add(senderIns);
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
/// 一定要重写!!!!!
///  用于生成初始化senders的初始位置合senders的类型
/// @datas: 传入的是 SenderInitDatas
void AHSenderPattern::SetSendersInitData_Implementation(TArray<FSenderInitData>& datas)
{
}
void AHSenderPattern::OnSampleBulletClone(AHBulletBase* YX, FName bulletName, AHBulletBase* cloneIns)
{
	CurBulletNums++;
	if(CurBulletNums%SendersIns.Num()==0)
	{
		OnFullShootDelegate.Broadcast(this,SampleBullet,GetOwner());
	}
	
}

void AHSenderPattern::OpenSenders()
{
	ensure(SendersIns.IsValidIndex(0));
	for(auto ins:SendersIns)
	{
		ins->OpenSender(GetInstigator());
	}
	
}

void AHSenderPattern::CloseSenders()
{
	ensure(SendersIns.IsValidIndex(0));
	for(auto ins:SendersIns)
	{
		ins->CloseSender(GetInstigator());
	}
	
}





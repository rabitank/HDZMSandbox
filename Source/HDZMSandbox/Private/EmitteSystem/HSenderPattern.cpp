// Fill out your copyright notice in the Description page of Project Settings.


#include "EmitteSystem/HSenderPattern.h"
#include "EmitteSystem/HSender.h"

// Sets default values
AHSenderPattern::AHSenderPattern()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AHSenderPattern::InitPattern()
{

	for (TSubclassOf<AHSender> senderC : Senders)
	{

		FActorSpawnParameters spawnParams;

		//TODO: SetHEmitterPatterns Insitgator as Player;And other set;
		spawnParams.Instigator = GetInstigator();
		spawnParams.Owner = this;

		AHSender* senderIns = GetWorld()->SpawnActor<AHSender>(senderC, spawnParams);

		senderIns->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

		SendersIns.Add(senderIns);

	}

}


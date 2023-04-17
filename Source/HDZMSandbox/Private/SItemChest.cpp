// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemChest.h"
#include "Net/UnrealNetwork.h"

//will happened in server
void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	bIsLipopened = !bIsLipopened ;
	OnRep_IsOpended();
}

//OnRep_IsOpended 链接的属性 replicated 发生时 会在被replicate的一方(client)调用该方法. server上如需要请手动调用
void ASItemChest::OnRep_IsOpended()
{
	float curPitch = bIsLipopened ? TargetPitch : 0.f;
	ComLidMesh->SetRelativeRotation(FRotator(curPitch, 0.f, 0.f));
}

// Sets default values
ASItemChest::ASItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ComBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("SItCComBaseMesh");
	RootComponent = ComBaseMesh;
	ComLidMesh = CreateDefaultSubobject<UStaticMeshComponent>("SItCComLidMesh");
	ComLidMesh->SetupAttachment(ComBaseMesh);

	TargetPitch = 110.f;

	//server will 'allow' copy actor replicated statues to client
	SetReplicates(true);
}


// Called when the game starts or when spawned
void ASItemChest::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//生存时间内获得复制(需要同步)的属性, 必须提供实现(提供如何同步方法)
void ASItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//do reply lifetime ,当在server中更新时发送/复制到client
	//需要replicated 属性
	DOREPLIFETIME(ASItemChest, bIsLipopened);
	
}


// Fill out your copyright notice in the Description page of Project Settings.



#include "LineSTG/HBulletBase.h"

#include "GameFramework/Actor.h"
#include "HPlayerCharacter.h"
#include "../HDZMSandbox.h"
#include "Components/StaticMeshComponent.h"
#include "HGameModeFunctionLibrary.h"
#include "K2Node_SpawnActor.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnitConversion.h"

unsigned int AHBulletBase::SpawnCount =0;

// Sets default values
AHBulletBase::AHBulletBase()
{

	SpawnCount++;
	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	ComRoot = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(ComRoot);

	ComMesh = CreateDefaultSubobject<UStaticMeshComponent>("BulletMesh");
	ComMesh->SetupAttachment(RootComponent);

	//Created CollisionProfile "Projectile" in Engine-Collision
	ComMesh->SetCollisionProfileName("Bullet");
	ComMesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	ComMesh->OnComponentHit.AddDynamic(this, &AHBulletBase::OnActorHit);
	


	//collision APi etc:
	//ComSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	//ComSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	//more information view:https://www.unrealengine.com/zh-CN/blog/collision-filtering
	
	bCanFadeAway = false;

//	ComEffectParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SProComParticleSystem"));
//	ComEffectParticle->SetupAttachment(RootComponent);
}

inline void AHBulletBase::SetFadeTimerHandle()
{
	if(bCanFadeAway)
		GetWorld()->GetTimerManager().SetTimer(TimeHandle_FadeAway,this,&AHBulletBase::FadeAway,LifeDuration,false);
	else
		GetWorld()->GetTimerManager().SetTimer(TimeHandle_FadeAway,this,&AHBulletBase::Exploed,LifeDuration,false);

	
}

// Called when the game starts or when spawned
void AHBulletBase::BeginPlay()
{
	Super::BeginPlay();
	SetFadeTimerHandle();
}

void AHBulletBase::OnActorHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& hit)
{
	//temp 
	//@TODO: 将来交给Manager管理.
	if (OtherActor && OtherActor == GetInstigator())
	{
		return;
	}
	Exploed();
}


void AHBulletBase::FadeAway_Implementation()
{
	//if call fadeAway in error;
	if (!ensureMsgf(bCanFadeAway,TEXT("Don't Call FaedAway: bCanFadeAway is false!")))
	{
		Exploed();
		return;
	}

	LogOnScreen(this,TEXT("a Bullet FadeAway!"),FColor::White,1.f);


	//@TODO Already: 通过设置spanlife来回收,而不是Destroy, Destory将在内存上删除actor,而不是进入回收机制循环 .
	// Ue https://docs.unrealengine.com/4.26/zh-CN/ProgrammingAndScripting/ProgrammingWithCPP/UnrealArchitecture/Actors/ActorLifecycle/
	// Call Destroy() Event -> EndPlay()💦.
	if (ensure(!IsPendingKill()))
	{
		GetWorld()->GetTimerManager().ClearTimer(TimeHandle_FadeAway);
		Destroy();
	}
}



bool AHBulletBase::ResetFadeTime(float time)
{
	if(bIsFrozen)
	{
		LifeDuration = time;
		return true;
	}
	
	if( GetWorld()->GetTimerManager().IsTimerPending(TimeHandle_FadeAway))
	{
		LifeDuration = time;
		GetWorld()->GetTimerManager().ClearTimer(TimeHandle_FadeAway);
		SetFadeTimerHandle();
		return true;
	}
	return false;
}

/// 冻结, 意味停止运动更新
/// @return 
bool AHBulletBase::Frozen()
{
	if(IsPendingKill()) return false;
	if (GetWorld()->GetTimerManager().IsTimerPending(TimeHandle_FadeAway))
	{
		bIsFrozen = true;
		return true;
	}
	return  false;
}

/// 原型化, 取消生命周期和移动更新. 作为克隆工具 
/// @return 原型化是否成功?
bool AHBulletBase::PrototypingYXH()
{
	if(Frozen())
	{
		GetWorld()->GetTimerManager().ClearTimer(TimeHandle_FadeAway);
		return true;
	}
	return false;
}

void AHBulletBase::Exploed_Implementation()
{
	LogOnScreen(this,TEXT("a Bullet Exploed!"),FColor::White,1.f);
	if (ensure(!IsPendingKill()))
	{
		GetWorld()->GetTimerManager().ClearTimer(TimeHandle_FadeAway);
		Destroy();
	}

}

// Called every frame
void AHBulletBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(bIsFrozen)
	{
		return;
	}
	Drive(DeltaTime);
}
inline void AHBulletBase::Drive_Implementation(float deltaTime)
{
	
}

/// 在蓝图/子类中重写记得调用super.
/// @param initor 另一个同类型bullet原型.
void AHBulletBase::Init_Implementation(AHBulletBase* initor)
{
	UE_LOG(LogTemp,Warning,TEXT("BUllet %s %d is Initing!"),*BulletName.ToString() , SpawnCount);
	LifeDuration = initor->LifeDuration;
	//Instigator 传递
	SetInstigator(initor->GetInstigator());
}

/// 克隆到. 即Spawn并init. 克隆行为由init(AB* )实现确定.
/// @param cloneLocation 位置
/// @param cloneRot 方向
/// @return 新子弹的引用
AHBulletBase* AHBulletBase::CloneTo_Implementation(FVector cloneLocation,FRotator cloneRot)
{
	SpawnCount++;
	const auto spawnTransform =  FTransform(cloneRot,cloneLocation);
	auto cloneName =  FString::Printf(TEXT("%s_%d"),*BulletName.ToString(),SpawnCount);
	//AHBulletBase* cloneBullet =Cast<AHBulletBase>( DuplicateObject(this,this->GetOuter(),FName(*cloneName)));

	AHBulletBase* cloneBullet = Cast<AHBulletBase>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, this->GetClass(),spawnTransform,ESpawnActorCollisionHandlingMethod::AlwaysSpawn )); //GetClass 获取动态类型
	//初始化传参
	cloneBullet->Init(this);
	//产生阶段结束
	UGameplayStatics::FinishSpawningActor(cloneBullet, spawnTransform);
	if (cloneBullet)
	{
		OnBulletClone.Broadcast(this,BulletName,cloneBullet);
		return  cloneBullet;
	}
	UE_LOG(LogTemp,Warning,TEXT("BUllet %s %d CloneFailed!"),*BulletName.ToString() , --SpawnCount);
	return  nullptr;
}


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HBulletBase.generated.h"

class AHBulletBase;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FCloneBulletDelegate, AHBulletBase*, YX, FName, bulletName, AHBulletBase* ,cloneIns);
UCLASS()
class HDZMSANDBOX_API AHBulletBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHBulletBase();

private:
	void SetFadeTimerHandle();
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UStaticMeshComponent* ComMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class USceneComponent* ComRoot;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
		float BulletHurt{1.f};

	static unsigned int SpawnCount ;

protected:
	//UPROPERTY(EditDefaultsOnly, Category = "Bullet")
	//	class UCurveFloat* ContainEnergyCurve;


	//让bullet仅持有必要的运动数据, 剩下的交由Name调用其他.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet")
	FName BulletName;
	
	/// bIsFrozen不参与拷贝
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly, Category = "Bullet | Movement")
	bool bIsFrozen{false};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bullet")
		float LifeDuration{10.f};


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bullet")
	FTimerHandle TimeHandle_FadeAway;
	
	//if true ,FadeAway really fadeaway after life ending ,false means use Exploed in FadeAway
	//@DefaultValue: false
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet")
	bool bCanFadeAway;

	UFUNCTION()
	void OnActorHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& hit);

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void FadeAway();
	
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void Exploed();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	AHBulletBase* CloneTo(FVector cloneLocation,FRotator cloneRot);

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void Init(AHBulletBase* initor);

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void Drive(float deltaTime);

	
	/// 重新设置生命周期时,请使用此函数
	/// @param time 
	/// @return 设置是否成功
	UFUNCTION(BlueprintCallable)
	bool ResetFadeTime(float time);

	UFUNCTION(BlueprintCallable)
	bool Frozen();

	UFUNCTION(BlueprintCallable)
	bool PrototypingYXH();
	
	FCloneBulletDelegate OnBulletClone;

};

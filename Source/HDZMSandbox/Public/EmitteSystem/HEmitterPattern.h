// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EmitteSystem/HSenderPattern.h"
#include "Delegates/DelegateCombinations.h"
#include "HEmitterPattern.generated.h"

/**
 * 
 */


UCLASS(BlueprintType)
class HDZMSANDBOX_API AHEmitterPattern : public AHSenderPattern
{
	GENERATED_BODY()
	
protected:


	/// <summary>
	/// 应该根据sender的子弹信息计算出来而不是自己设置
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float DesireEngery{ 0.f };
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bIsShoot{ false };
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bIsActive{ false };
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bShouldRecoil{false};
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
		int SendersNum{1};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName PatterName {TEXT("Default") };
	//原生子弹类型. player使用的emitterpattern的子弹类会较为恒定,从蓝图来生成Samplebullet并传递,该逻辑不能交给character来完成.
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
		TSubclassOf<AHBulletBase> NativeBulletType;
	
		
	
	int ShootedNum{ 0 };

	/// <summary>
	/// use it after (Init) InitOffsetAngular!
	/// </summary>
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly,Category = "Transform")
		bool bIsNeedOffset{false};
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly,Category = "Transform")
		float IntervalAngular{0.f};
	
	/// <summary>
	/// 暂时公开..大概,主要没想好pattern的运动
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "Transform")
		float SenderLength { 20.f };

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Sender")
		float PreSenderInterval{0.2f};
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void InitOffsetAngular();
	UFUNCTION(BlueprintCallable)
		void SetSenderLengths(float length );


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void InitEmitterPattern();
	
	/// <summary>
	/// 使sender对准PatternX方向上的一个位置. 
	/// </summary>
	UFUNCTION(BlueprintCallable)
		void SetSenderRotateTo(float length );

	
	UFUNCTION(BlueprintCallable)
		bool IsActivePattern() const { return bIsActive; };
	UFUNCTION(BlueprintCallable)
		bool IsShooting() const { return bIsShoot; };
	

	UFUNCTION(BlueprintNativeEvent)
		void UpdateSendersTransform();

	UFUNCTION(BlueprintNativeEvent)
		bool Shoot(AActor* instagor);
	
	UFUNCTION(BlueprintNativeEvent)
		void StopShoot(AActor* instagor);
	
	UFUNCTION(BlueprintNativeEvent)
		void PatternRelax();
	UFUNCTION(BlueprintNativeEvent)
		void PatternActive();
	

	/// <summary>
	/// 呃呃呃,扣点能量,计数添加后坐力啥的 -> 都交给Manage
	/// </summary>
	UFUNCTION(BlueprintCallable)
		void OnFullShoot(AHSenderPattern* emitterpattern,AHBulletBase* sampleBulle,AActor* senderOwner );
	
	UFUNCTION(BlueprintCallable)
	inline FName GetPatternName() const { return PatterName; };
	
	void SetShouldRecoil(bool should) { bShouldRecoil = should; };



	virtual void SetSendersInitData_Implementation(TArray<FSenderInitData>& datas) override;;  
};

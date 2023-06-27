// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EmitteSystem/HSenderPattern.h"
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
	void InitPattern() override;
	
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
	
	
	


};

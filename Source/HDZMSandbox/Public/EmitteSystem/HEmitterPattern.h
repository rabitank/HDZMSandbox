// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EmitteSystem/HSenderPattern.h"
#include "HEmitterPattern.generated.h"

/**
 * 
 */
UCLASS()
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


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void InitPattern() override;
	
	
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

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
	/// Ӧ�ø���sender���ӵ���Ϣ��������������Լ�����
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
	//ԭ���ӵ�����. playerʹ�õ�emitterpattern���ӵ�����Ϊ�㶨,����ͼ������Samplebullet������,���߼����ܽ���character�����.
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
	/// ��ʱ����..���,��Ҫû���pattern���˶�
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
	/// ʹsender��׼PatternX�����ϵ�һ��λ��. 
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
	/// ������,�۵�����,������Ӻ�����ɶ�� -> ������Manage
	/// </summary>
	UFUNCTION(BlueprintCallable)
		void OnFullShoot(AHSenderPattern* emitterpattern,AHBulletBase* sampleBulle,AActor* senderOwner );
	
	UFUNCTION(BlueprintCallable)
	inline FName GetPatternName() const { return PatterName; };
	
	void SetShouldRecoil(bool should) { bShouldRecoil = should; };



	virtual void SetSendersInitData_Implementation(TArray<FSenderInitData>& datas) override;;  
};

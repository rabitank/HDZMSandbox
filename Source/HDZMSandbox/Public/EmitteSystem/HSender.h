// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HSender.generated.h"

UCLASS()
class HDZMSANDBOX_API AHSender : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHSender();

private:
	float CurRelaxTime{0.f};

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UStaticMeshComponent* ComMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class USceneComponent* ComRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		class AHBulletBase* SampleBullet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		float Interval{1.f};
	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings| BulletSetting")
		bool bOpenShooting{false};

	UFUNCTION()
	AHBulletBase* SpawnBullet(FVector loc,FRotator dir) const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void Shooting();

	
	UFUNCTION(BlueprintNativeEvent)
		void UpdateSenderBehaviour(float DeltaTime);
	UFUNCTION(BlueprintCallable)
		void OpenSender(AActor* instagor);
	UFUNCTION(BlueprintCallable)
		void CloseSender(AActor* instagor);
	
	UFUNCTION(BlueprintCallable)
		void ResetInterval(float newInterval);
	UFUNCTION(BlueprintCallable)
		void ResetSampleBullet(AHBulletBase* sample);

	
	UFUNCTION(BlueprintCallable)
	void InitSender(AHBulletBase* sample);
	UFUNCTION(BlueprintCallable)
	void InitSender_IS(float senderInterval,AHBulletBase* sample);
	///ÒÑÆúÓÃ
	UFUNCTION(BlueprintCallable)
	void InitSender_IPR(float senderInterval,FVector relativePosition,FRotator relativeRot);

	UFUNCTION(BlueprintCallable)
	void SetSenderRelativeTransform(FVector relativePosition, FRotator relativeRot);

	friend class AHSenderPattern;
	friend class AHEmitterPattern;
	
};

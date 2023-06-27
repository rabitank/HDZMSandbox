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
	/// <summary>
	/// 是否是世界中作为机关放置的Emitter
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
		bool  bWorldSender{false};

	/*
	* SenderSetting 里的一些东西似乎有些多余
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings| SenderSetting")
		class UHSenderSettings* SenderSettings;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings| BulletSetting")
		bool bRecoverBulletBehaviour{false};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings| BulletSetting")
		float BulletVelocity{500.f};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings| BulletSetting")
		float BulletVelAce {0.f};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings| BulletSetting")
		float BulletAngAce{0.f};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings| BulletSetting")
		float BulletAngVel{5.f};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings| BulletSetting")
		bool bOpenShooting{false};



	UFUNCTION()
	void SpawnBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void InitSender();
	void Shooting();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent)
		void InitBullet(AHBulletBase* bullet);
	
	UFUNCTION(BlueprintNativeEvent)
		void UpdateSenderBehaviour(float DeltaTime);
	
	UFUNCTION(BlueprintCallable)
		void OpenSender(AActor* instagor);
	UFUNCTION(BlueprintCallable)
		void CloseSender(AActor* instagor);

	void InitSender(FVector relativePositon,FRotator relativeRot);

	//偷懒*2
	friend class AHCard;
	friend class AHEmitterPattern;



};

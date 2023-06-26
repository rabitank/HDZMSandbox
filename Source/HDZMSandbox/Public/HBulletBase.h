// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HBulletBase.generated.h"

UCLASS()
class HDZMSANDBOX_API AHBulletBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHBulletBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UStaticMeshComponent* ComMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class USceneComponent* ComRoot;
	
protected:
	//UPROPERTY(EditDefaultsOnly, Category = "Bullet")
	//	class UCurveFloat* ContainEnergyCurve;


	//让bullet仅持有必要的运动数据, 剩下的交由Name调用其他.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet")
	FName BulletName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bullet | Movement")
	float LinearVelocity;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bullet | Movement")
	float LinearAcceleration;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bullet | Movement")
	float MaxLinearVeclocity;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Bullet | Movement")
	float AngularVelocity;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bullet | Movement")
	FVector AngularAxis {-1.f,0.f,0.f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bullet | Movement")
	float AngularAcceleration{0.f};
	

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

	UFUNCTION(BlueprintNativeEvent)
	void FadeAway();
	
	UFUNCTION(BlueprintNativeEvent)
	void Exploed();


	virtual void UpdateVelcoity(float DeltaTime);
	virtual void UpdateAccelreation(float DeltaTime) {};
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	//	class UParticleSystemComponent* ComEffectParticle;
	//
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	//	class UParticleSystem* ExplodeParticle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	inline void SetLinearVelocity(float LV) { LinearVelocity = LV; };
	inline void SetLinearAcceleration(float LA) { LinearAcceleration = LA; };
	inline void SetAngularVeloctiy(float AV) { AngularVelocity= AV; };
	inline void SetAngularAcceleration(float AA) { AngularAcceleration= AA; };

};

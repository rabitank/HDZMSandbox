// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HBulletBase.h"
#include "HLineProcessInterface.h"
#include "HComplexBullet.generated.h"

/**
 * 
 */
class UHDriver;
UCLASS(Blueprintable)
class HDZMSANDBOX_API AHComplexBullet : public AHBulletBase,public IHLineProcessInterface
{
	GENERATED_BODY()

protected:
	AHComplexBullet();
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Movement | Driver")
	UHDriver* Driver;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Movement | Driver")
	TSubclassOf<UHDriver> NativeDriverClass;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Drive_Implementation(float deltaTime) override;
	virtual void Init_Implementation(AHBulletBase* initor) override;

	virtual void BeginPlay() override;
	virtual AHBulletBase* CloneTo_Implementation(FVector cloneLocation, FRotator cloneRot) override;

	UFUNCTION(BlueprintCallable)
	UHDriver* GetDriverRef() const {return Driver;} ;
	friend UHDriver;
};

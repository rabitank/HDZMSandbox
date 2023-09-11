// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HDriver.generated.h"

class AHComplexBullet;
UCLASS(Blueprintable)
class HDZMSANDBOX_API UHDriver : public UObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UHDriver();

	
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TSubclassOf<AHComplexBullet> OwningerClass;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	AHComplexBullet* Owninger{nullptr};

	static  unsigned int CreatedCount;

public:	
	// Called every frame
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void DriverUpdate(float DeltaTime) ;
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	UHDriver* CloneToAttach(AHComplexBullet* beDrivinger);

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void InitAttachment(AHComplexBullet* attachment);

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	bool ResetAttachment(AHComplexBullet* attachment);
	
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	UHDriver* CloneOne() ;

	friend AHComplexBullet;
};

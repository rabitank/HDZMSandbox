// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "HEmitAction.generated.h"

/**
 * 
 */
USTRUCT()
struct FEmitData
{
	GENERATED_BODY()
public:

	UPROPERTY()
		bool bIsRunning = false;
	UPROPERTY()
		AActor* Instigator;

};

UCLASS(Blueprintable)
class HDZMSANDBOX_API UHEmitAction : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY()
		class UHEmitterComponent* EmitterCompnent;

	UPROPERTY(VisibleAnywhere, Category = "Action")
		FEmitData EmitActionData;

	UPROPERTY()
		float StartedTime;
	
	UPROPERTY(EditAnywhere,Category = "Action")
		float Duration;


public:
	void Initialize(class UHEmitterComponent* NewEmitterComp);

	//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	//		class UTexture2D* ActionIcon;

	UHEmitAction();

	UFUNCTION(BlueprintNativeEvent, Category = "Emit")
		void StartShoot(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Emit")
		void StopShoot(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Emit")
		bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Emit")
		bool IsShooting() { return EmitActionData.bIsRunning; };


	UPROPERTY(VisibleAnywhere, Category = "Emit")

		FName AcitonName;
	UFUNCTION(BlueprintCallable, Category = "Emit")
		class UHEmitterComponent* GetOwningComponent() const;

	virtual class UWorld* GetWorld() const override;

};



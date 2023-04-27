// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "HAction.generated.h"

/**
 * 
 */

USTRUCT()
struct FHActionData
{
	GENERATED_BODY()
public:

	UPROPERTY()
		bool bIsRunning = false;
	UPROPERTY()
		AActor* Instigator;

};

UCLASS(Blueprintable)
class HDZMSANDBOX_API UHAction : public UObject
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
		class UHActionComponent* ActionComp;

	UPROPERTY(VisibleAnywhere, Category = "Action")
		FHActionData ActionData;

	UPROPERTY()
		float StartedTime;

public:
	virtual void Initialize(UHActionComponent* NewActionComp);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
		class UTexture2D* ActionIcon;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
		bool bIsAutoStart;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
		void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
		void StopAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
		bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Action")
		bool IsRunning();

	UHAction();

	UPROPERTY(EditDefaultsOnly, Category = "Action")
		FName AcitonName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
		FGameplayTagContainer GrantTags;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
		FGameplayTagContainer BlockTags;


	UFUNCTION(BlueprintCallable, Category = "Action")
		class UHActionComponent* GetOwningComponent() const { return ActionComp; };

	virtual class UWorld* GetWorld() const override;

};

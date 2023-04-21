// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "SAction.generated.h"
/**
 * 
 */

USTRUCT()
struct FActionData
{
	GENERATED_BODY()
public:

	UPROPERTY()
		bool bIsRunning = false;
	UPROPERTY()
		AActor* Instigator;

};

UCLASS(Blueprintable)
class HDZMSANDBOX_API USAction : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(Replicated)
		class USActionComponent* ActionComp;

	UPROPERTY(VisibleAnywhere,ReplicatedUsing = "OnRep_ActionData",Category = "Action")
		FActionData RepActionData;
	//for contiuing action,action would not auto intervally call stopAction , character may try to stopAction without startAction,
	//and to avoid excuting new action without another action end, should to contain action bisrunning

	//@Really?
	//AActor's Spawn is defaulty be replicated to client
	//but SAction is subclass of UO, its creation would be replicated to client in default case
	
	UFUNCTION()
		void OnRep_ActionData();

	UPROPERTY(Replicated)
		float StartedTime;

public:
	void Initialize	(USActionComponent* NewActionComp);



	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
		class UTexture2D* ActionIcon;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
		bool bIsAutoStart;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
		void StartAction(AActor* Instigator);
	
	UFUNCTION(BlueprintCallable ,BlueprintNativeEvent, Category = "Action")
		void StopAction(AActor* Instigator);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
		bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Action")
		bool IsRunning();

	USAction();

	UPROPERTY(EditDefaultsOnly, Category = "Action")
		FName AcitonName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
		FGameplayTagContainer GrantTags;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
		FGameplayTagContainer BlockTags;


	UFUNCTION(BlueprintCallable, Category = "Action")
	class USActionComponent* GetOwningComponent() const;

	virtual class UWorld* GetWorld() const override;
	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	};
	
};

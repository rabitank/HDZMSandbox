// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Delegates/DelegateCombinations.h"
#include "GameplayTagContainer.h"
#include "HActionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHActionStatedChanged, class UHActionComponent*, ActionComp, class UHAction*, action);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HDZMSANDBOX_API UHActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHActionComponent();

	UFUNCTION(BlueprintCallable, Category = "Actions")
		void AddAction(AActor* Instigator, TSubclassOf<UHAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category = "Actions")
		class UHAction* GetAction(TSubclassOf<class UHAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category = "Actions")
		bool StartActionByName(AActor* Instigator, FName actionName);

	UFUNCTION(BlueprintCallable, Category = "Actions")
		bool  StopActionByName(AActor* Instigator, FName actionName);

	UFUNCTION(BlueprintCallable, Category = "Actions")
		void RemoveAction(UHAction* actionToRemove);

public:

	UFUNCTION(BlueprintCallable, Category = "Attribute")
		static UHActionComponent* GetActions(AActor* fromactor);



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
		FGameplayTagContainer ActiveGameplayTags;

	UPROPERTY(BlueprintAssignable)
		FOnHActionStatedChanged OnActionStarted;

	UPROPERTY(BlueprintAssignable)
		FOnHActionStatedChanged OnActionStoped;

protected:
	//参考架构 view:GAS:https://www.bilibili.com/video/BV1zD4y1X77M
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Actions")
		TArray<TSubclassOf<UHAction>> DefaultActions;

	UPROPERTY(BlueprintReadOnly)
		TArray<UHAction*> Actions;


public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
};

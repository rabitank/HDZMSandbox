// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "Delegates/DelegateCombinations.h"
#include "SActionComponent.generated.h"

class USAction;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionStatedChanged, class USActionComponent*, ActionComp,USAction*, action);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HDZMSANDBOX_API USActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	

	UFUNCTION(BlueprintCallable, Category = "Actions")
		void AddAction(AActor* Instigator,TSubclassOf<USAction> ActionClass);
	
	UFUNCTION(BlueprintCallable, Category = "Actions")
		class USAction* GetAction(TSubclassOf<USAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StartActionByName( AActor* Instigator , FName actionName);
	
	
	UFUNCTION( Server,Reliable)
	void ServerStartActionByName( AActor* Instigator , FName actionName);
	
	UFUNCTION( Server,Reliable)
	void ServerStopActionByName( AActor* Instigator , FName actionName);

	
	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool  StopActionByName(AActor* Instigator, FName actionName);
	
	UFUNCTION(BlueprintCallable, Category = "Actions")
	void RemoveAction(USAction* actionToRemove);

public:

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	static USActionComponent* GetActions(AActor* fromactor);


	USActionComponent();

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Tags")
	FGameplayTagContainer ActiveGameplayTags;


	UPROPERTY(BlueprintAssignable)
	FOnActionStatedChanged OnActionStarted;

	UPROPERTY(BlueprintAssignable)
	FOnActionStatedChanged OnActionStoped;

protected:
	//参考架构 view:GAS:https://www.bilibili.com/video/BV1zD4y1X77M
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Actions")
	TArray<TSubclassOf<USAction>> DefaultActions;

	//角色动作/能力库 
	UPROPERTY( BlueprintReadOnly,Replicated)
	TArray<USAction*> Actions;


public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
		
};

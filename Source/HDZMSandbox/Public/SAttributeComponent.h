// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

//it defferent with DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_TParams声明动态多播稀疏代理  //声明动态多播代理, 你自己的代理签
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged,USAttributeComponent*,owningComp,AActor*,instigatorActor,float,newHealth, float ,delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HDZMSANDBOX_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USAttributeComponent();


protected:

	// EditAnywhere - edit in Bp editor and per-instance in level.'read-only' in editor and level. (use for components) visibleAnywhere -
	// EditDefaultsonly - hide variable per-instance, edit in Bp editor only// VisibleDefaultsonly"read-only' access for variable, only in Bp editor (uncommon)
	// EditInstanceOnly - allow only editing of instance (eg. when placed in level)
	// BlueprintReadonly - readonly in the Blueprint scripting (does not affect 'details'-panel
	// BlueprintReadwrite - read-write access in Blueprints
	// Category = " . display only for detail panels and blueprint context menu.

	//maybe need 0_<

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Attributes")
	float Health;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Attributes")
	float HealthMax;

	//healthMax,Stamina,Strength
	//Stamina:the physical and/or mental strength to do something that might be difficult and will take a long time


public:
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	UFUNCTION(BlueprintCallable)
	bool ApplyHealthChangeDelta(float delta, AActor* Instigator = nullptr);
	
	UFUNCTION(BlueprintCallable)
	float GetMaxHealth();
	
	UFUNCTION(BlueprintCallable)
	bool IsAlive();
	
	UFUNCTION(BlueprintCallable)
	bool IsLowHealth();

	UFUNCTION()
	inline	bool IsMaxHealth() { return Health == HealthMax; }

public:

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	static USAttributeComponent* GetAttributes(AActor* formActor) ;
	
	//DisplayName is name in bp
	UFUNCTION(BlueprintCallable, Category = "Attribute", meta = (DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* queryActor) ;


};

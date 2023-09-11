// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Delegates/DelegateCombinations.h"
#include "SAttributeComponent.generated.h"

//it defferent with DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_TParams声明动态多播稀疏代理  //声明动态多播代理, 你自己的代理签
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged,USAttributeComponent*,owningComp,AActor*,instigatorActor,float,newHealth, float ,delta);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged,class USAttributeComponent*,owningComp,AActor*,instigatorActor,float,newAttribute, float ,delta);

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

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Replicated,Category="Attributes")
	float Health;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Replicated,Category="Attributes")
	float HealthMax;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Replicated,Category="Attributes")
	float Rage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated,Category="Attributes")
	float RageMax;

	



	//healthMax,Stamina,Strength
	//Stamina:the physical and/or mental strength to do something that might be difficult and will take a long time

	UFUNCTION(NetMulticast, Reliable)
		void MultiCastOnHealthChanged(USAttributeComponent* owncomp, AActor* Instigator, float newHealth, float actualDelta);
	
	UFUNCTION(NetMulticast, Reliable)
		void MultiCastOnRageChanged(USAttributeComponent* owncomp, AActor* Instigator, float newRage, float actualDelta);


public:
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnRageChanged;

	UFUNCTION(BlueprintCallable)
	bool ApplyHealthChangeDelta(float delta, AActor* InstigatorActor);
	
	UFUNCTION(BlueprintCallable)
	bool ApplyRageChangeDelta(float delta, AActor* Instigator = nullptr);
	

	UFUNCTION(BlueprintCallable)
	bool Kill(AActor* Instigator);
	

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth();
	
	UFUNCTION(BlueprintCallable)
		float GetMaxRage() { return RageMax; };
	
	UFUNCTION(BlueprintCallable)
		float GetRage() { return Rage; };
	
	UFUNCTION(BlueprintCallable)
	bool IsAlive();

	UFUNCTION()
	inline	bool IsMaxHealth() { return Health == HealthMax; }
	
	UFUNCTION()
	inline	bool IsMaxRage() { return Rage == RageMax; }

public:

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	static USAttributeComponent* GetAttributes(AActor* formActor) ;

	
	//DisplayName is name in bp
	UFUNCTION(BlueprintCallable, Category = "Attribute", meta = (DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* queryActor) ;
	UFUNCTION()
	bool IsLowHealth() const { return Health / HealthMax < 0.4f; };
};

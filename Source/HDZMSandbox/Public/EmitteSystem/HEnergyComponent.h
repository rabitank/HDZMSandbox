// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Delegates/DelegateCombinations.h"
#include "HEnergyComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnEmitterStateChanged, class UHEnergyComponent*, owningComp, AActor*, instigatorActor, float, newAttribute, float, delta);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HDZMSANDBOX_API UHEnergyComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHEnergyComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
		float Energy;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
		float EnergyMax;
	//EnergyDangerou means absorbed to m
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
		float EnergyDangerou;

	UPROPERTY(BlueprintAssignable)
		FOnEmitterStateChanged OnEnergyChanged;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		float GetEnergy() { return Energy; }
	UFUNCTION(BlueprintCallable)
		bool IsAffordable(float DemandEnergy) const { return Energy + DemandEnergy >= 0; }
	UFUNCTION(BlueprintCallable)
		bool IsDangerouOrInit() const { return Energy >= EnergyDangerou; };

	UFUNCTION(BlueprintCallable)
		bool ApplyEnergyChangeDelta(AActor* Instigator, float delta);

	UFUNCTION(BlueprintCallable)
		void RestoreEnergyToInitLevel();

	static UHEnergyComponent* GetAttribute(AActor* formActor);


public:

	virtual void PostInitProperties() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

};

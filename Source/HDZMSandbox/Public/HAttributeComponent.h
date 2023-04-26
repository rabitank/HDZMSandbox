// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Delegates/DelegateCombinations.h"
#include "HAttributeComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnEnergyChanged, class UHAttributeComponent*, owningComp, AActor*, instigatorActor, float, newAttribute, float, delta);

static TAutoConsoleVariable<bool> CvarStaticEnergy(TEXT("hd.StaticEnergy"), false, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HDZMSANDBOX_API UHAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHAttributeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float Energy;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float EnergyMax;
	
	//EnergyDangerou means absorbed to m
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float EnergyDangerou;
	


	UPROPERTY(BlueprintAssignable)
		FOnEnergyChanged OnEnergyChanged;

public:	
	UFUNCTION(BlueprintCallable)
		bool ApplyEnergyChangeDelta(AActor* Instigator,float delta);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		float GetEnergy() { return Energy; }
	
	UFUNCTION(BlueprintCallable)
		bool IsAffordable(float DemandEnergy) const { return Energy - DemandEnergy >= 0; }

	UFUNCTION(BlueprintCallable)
		bool IsDangerouOrInit() const { return Energy >= EnergyDangerou; };


	UFUNCTION(BlueprintCallable)
	void RestoreEnergyToInitLevel();





	static UHAttributeComponent* GetAttribute(APawn* pawn);

		
};

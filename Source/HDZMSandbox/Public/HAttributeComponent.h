// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Delegates/DelegateCombinations.h"
#include "HAttributeComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnCharacterStateChanged, class UHAttributeComponent*, owningComp, AActor*, instigatorActor, float, newAttribute, float, delta);

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
		float Health{0.f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
		float HealthMax{100.f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
		float HealthDangerouPersent{ 0.2f };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
	float HealthDangerou;
	
	UPROPERTY(BlueprintAssignable)
		FOnCharacterStateChanged OnHealthChanged;


public:	

	UFUNCTION(BlueprintCallable)
		bool ApplyHealthChangeDelta(AActor* Instigator,float delta);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		float GetHealth() { return Health; }
	UFUNCTION(BlueprintCallable)
		bool IsHealthAffordable(float DemandHealth) const { return Health + DemandHealth>= 0; }
	UFUNCTION(BlueprintCallable)
		bool IsHealthDangerou() const { return Health >= HealthDangerou; };
	UFUNCTION(BlueprintCallable)
		bool IsHealthFull() const { return Health == HealthMax; };

	UFUNCTION(BlueprintCallable)
	void RestoreHealthToFull();

	virtual void PostInitProperties() override ;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	static UHAttributeComponent* GetAttribute(AActor* formActor);



};

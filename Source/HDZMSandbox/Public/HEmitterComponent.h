// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HEmitterCoreBase.h"
#include "HEmitterComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HDZMSANDBOX_API UHEmitterComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHEmitterComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Emitter")
		FName DefaultMuzzelSocketName;

	UPROPERTY(EditAnywhere, Category = "Emitter")
		int32 CoreSlotsNum;
	
	UPROPERTY(VisibleAnywhere, Category = "Emitter")
		class UHAttributeComponent* EnergySourceComp;


	UPROPERTY(EditAnywhere, Category = "Emitter")
		TArray<TSubclassOf<UHEmitterCoreBase>> DefaultEmitterCoreClass;

	UPROPERTY(VisibleAnywhere,Category = "Emitter")
	TArray<UHEmitterCoreBase*> EmitterCores;
	
	UPROPERTY(VisibleAnywhere, Category = "Emitter")
	UHEmitterCoreBase* CurrentCore;
	
	UPROPERTY(VisibleAnywhere, Category = "Emitter")
	float CurrentCoreIndex;

public:

	//return UHEC for showing Info of Core when changed to an new core
	//for user use mouse slide to change core
	UFUNCTION(BlueprintCallable)
	UHEmitterCoreBase* SlideCore(float val);
	
	//for user use numpad to change core
	UFUNCTION(BlueprintCallable)
	UHEmitterCoreBase* SelectedCoreByNum(int val);

	UFUNCTION(BlueprintCallable)
		UHEmitterCoreBase* GetCurrentCore() { return CurrentCore; };
	

	UFUNCTION()
	FTransform GetDefaultMuzzleTransform();

	UFUNCTION(BlueprintCallable, Category = "Emitter")
		void AddCore(AActor* Instigator, TSubclassOf<UHEmitterCoreBase> CoreClass);

	UFUNCTION(BlueprintCallable, Category = "Emitter")
		class UHEmitterCoreBase* GetCore(TSubclassOf<UHEmitterCoreBase> CoreClass);

	//UFUNCTION(BlueprintCallable, Category = "Actions")
	//bool StopCoreByName(AActor* Instigator, FName CoreName);
	

	//@TODO:
	//for a core need pressed the key for a short time, when stop pressed before spawn bullet can stop the emitting
	//wait for more complexed core to use
	UFUNCTION(BlueprintCallable, Category = "Emitter")
	bool StopCurrentCoreEmit(AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Emitter")
	void RemoveCore(UHEmitterCoreBase* CoreToRemove);


public:
	UFUNCTION(BlueprintCallable)
		void EmitCurrentBullet(AActor* instigator);

	static UHEmitterComponent* GetEmitter(AActor* actor);


	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool bIsShooting;

		
};

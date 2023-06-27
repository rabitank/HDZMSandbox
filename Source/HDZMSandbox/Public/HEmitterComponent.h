// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HEmitterCoreBase.h"
#include "Delegates/DelegateCombinations.h"
#include "HEmitterComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnShootingStateChanged, class AHEmitterPattern*, thisCore, class AActor*, instigatorActor);

class AHEmitterPattern;

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

	UPROPERTY(EditDefaultsOnly,  Category = "Emitter| Init")
	TSubclassOf<AHEmitterPattern> BKEpClass ;
	UPROPERTY(EditDefaultsOnly, Category = "Emitter| Init")
	TSubclassOf<AHEmitterPattern> FWEpClass ;

	
	UPROPERTY(VisibleAnywhere, Category = "Emitter")
	AHEmitterPattern* CurrentPattern;
	UPROPERTY(VisibleAnywhere, Category = "Emitter")
	AHEmitterPattern* BKEmittePattern;
	UPROPERTY(VisibleAnywhere, Category = "Emitter")
	AHEmitterPattern* FWEmittePattern;
	
	/// <summary>
	/// ָforward;BKp��������
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category ="Emitter")
		bool bShouldAim{ false };

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "Emitter")
	class AHEmitter* OwnerEmitter ;


	UPROPERTY(VisibleAnywhere, Category = "Emitter")
		bool bIsTriggering{false};
	UPROPERTY(VisibleAnywhere, Category = "Emitter")
		bool bAbleShoot{false};

public:


	/// <summary>
	/// @����: ����Bk �� FWʹ�õ�Ep. ����ܳ��õ�
	/// </summary>
	UFUNCTION(BlueprintCallable ,Category="EmitterModify")
	void SwitchPattern();
	
	/// <summary>
	/// @����: �л���׼״̬, ����ӦEmitter��Aiming. ����ĳFW/BK
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void OnSwitchAimingState();
	
	/// <summary>
	/// @����: �л�FW��Ep, ���ı�FWEp.
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "EmitterModify")
	bool ChangeFWEp(TSubclassOf<AHEmitterPattern> newPattern,AActor* instigator);
	
	/// <summary>
	/// @����:���￪��.
	/// </summary>
	/// <param name="instegator"></param>
	/// <returns> �����Ƿ�ͨ�� </returns>
	UFUNCTION(BlueprintCallable)
	bool Shoot(AActor* instegator);

	UFUNCTION()
		void DelayInitPattern_Elaps (AHEmitterPattern* EpIns);


	//class UHEmitterPattern* GetPattern(TSubclassOf<UHEmitterPattern> PatternClass);
	

	//UFUNCTION(BlueprintCallable, Category = "Actions")
	//bool StopCoreByName(AActor* Instigator, FName CoreName);

	//@TODO:
	//for a core need pressed the key for a short time, when stop pressed before spawn bullet can stop the emitting
	//wait for more complexed core to use

	UFUNCTION(BlueprintCallable, Category = "Emitter")
	void RemoveCore(UHEmitterCoreBase* CoreToRemove);


public:
	UFUNCTION(BlueprintCallable)
	void OnTrigerPressed(AActor* instigator);
	
	UFUNCTION(BlueprintCallable)
	void OnTrigerReleased(AActor* instigator);

	inline void SetTriggerState(bool newState) {	bIsTriggering = newState;};
	inline bool GetTriggerState() {	return bIsTriggering ;};

	static UHEmitterComponent* GetEmitter(AActor* actor);

	UFUNCTION(BlueprintCallable)
		void SetAimingTarget(bool shouldOpenAim) { bShouldAim = shouldOpenAim; };



	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/// <summary>
	/// ��ʱ�����.������Ҫ������Ӧ����̫�涯��.
	/// </summary>
	FOnShootingStateChanged OnShootStarted;
	FOnShootingStateChanged OnShootStoped;

	UFUNCTION(BlueprintCallable)
		bool SpawnAndInitPatternWithDelay(TSubclassOf<AHEmitterPattern> pattern, AHEmitterPattern*& patternIns,float delay=0.2f);
		
};

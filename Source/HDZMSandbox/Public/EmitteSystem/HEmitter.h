// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "State/HShootType.h"
#include "HEmitter.generated.h"

UCLASS(Blueprintable)
class HDZMSANDBOX_API AHEmitter : public AActor
{
	GENERATED_BODY()
	

private:

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UArrowComponent* ComEmitterDirection;
	

protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Components")
		class USkeletalMeshComponent* ComEmitterMesh;


	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Components")
		class UHEnergyComponent* ComEnergy;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Components")
		class UHEmitterComponent* ComHEmitter;

	//initialize in native already!
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Reference")
	class APlayerController* OwnerController;
 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Reference")
	class AHPlayerCharacter* OwnerPawn;

	
	UPROPERTY(BlueprintReadOnly, Category = "Pawn States")
		bool  bIsAim{false};
	UPROPERTY(BlueprintReadOnly, Category = "Pawn States")
		bool  bPreIsAim{false};
	UPROPERTY(BlueprintReadOnly, Category = "Pawn States")
		bool  bHasMovementInput{false};


	UPROPERTY(BlueprintReadOnly, Category = "Pawn States")
		FRotator  CharacterRotator{0.f,0.f,0.f};
	UPROPERTY(BlueprintReadOnly, Category = "Pawn States")
		EHShootType  ShootType {EHShootType::None};
	
	UPROPERTY(BlueprintReadOnly, Category = "Pawn States")
		FVector  InputAcceleration {FVector::ZeroVector};
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Config")
		class UHEmitterSettings* ESPMBaseSetting;
	
	

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Config")
		float  EmitterRadius {57.f};
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Config")
		float  PitchSpeed{5.f};

	//遇到阻碍emitter改变距离轴心的距离时的速度.
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Config")
		float  AdaptDistSpeed{10.f};
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Config")
		FVector LagSpeed {19,19,20};/*= FVector(14,14,16) */

	UPROPERTY(BlueprintReadOnly, Category = "Anim")
		float  DeltaSeconds{0.001f};

	UPROPERTY(BlueprintReadOnly, Category = "Anim")
		FRotator  TargetRotator;
	UPROPERTY(BlueprintReadOnly, Category = "Anim")
		FRotator  SmoothedRotator {0.f,0.f,0.f};
	UPROPERTY(BlueprintReadOnly, Category = "Anim")
		FRotator  AimingRotator {0.f,0.f,0.f};
	

	UPROPERTY(BlueprintReadOnly, Category = "Anim")
		float  AimingDistance {0.f};



	UPROPERTY(BlueprintReadOnly, Category = "Anim")
		float CurDistance;
	UPROPERTY(BlueprintReadOnly, Category = "Anim")
		float TargetDistance;
	UPROPERTY(BlueprintReadOnly, Category = "Anim")
		FVector SmoothPivotLocation {0.f,0.f,0.f};

	UPROPERTY(BlueprintReadOnly, Category = "Anim|CameraState")
		FVector CameraLocation;
	UPROPERTY(BlueprintReadOnly, Category = "Anim|CameraState")
		FRotator CameraRotation;

	UPROPERTY(BlueprintReadOnly, Category = "Anim")
		float SmoothedRotatorYaw;
	UPROPERTY(BlueprintReadOnly, Category = "Anim")
		float EnteringAimingPitch;
	UPROPERTY(BlueprintReadOnly, Category = "Anim")
		float FinalScaleFac;

	UPROPERTY(VisibleDefaultsOnly, Category = "Config | CurveName")
		FName Curve_DistanceToPivot {"Emitter_DistanceToPivot"};
	UPROPERTY(VisibleDefaultsOnly, Category = "Config | CurveName")
		FName Curve_PivotXOffset {"Emitter_Pivot_XOffset"};
	UPROPERTY(VisibleDefaultsOnly, Category = "Config | CurveName")
		FName Curve_PivotYOffset{"Emitter_Pivot_YOffset"};
	UPROPERTY(VisibleDefaultsOnly, Category = "Config | CurveName")
		FName Curve_PivotZOffset {"Emitter_Pivot_ZOffset"};
	UPROPERTY(VisibleDefaultsOnly, Category = "Config | CurveName")
		FName Curve_RotatorSpeed {"Emitter_RotateSpeed"};
	UPROPERTY(VisibleDefaultsOnly, Category = "Config | CurveName")
		FName Curve_ScaledFac{"Emitter_ScaledFac"};




private:

	void UpdateEssentialValues();
	void UpdateEmitterBehaviour();
	void UpdateAimRotator(FVector CurrentTargetPivotLocation);

	void InitInputBind();


	void OnWheelSlide(float val);

	FRotator GetAimBackRotatorWithMoveInputFac();
	FRotator GetAimBackRotatorWithControlFac();


	FVector CaculateAxisIndepentLag(FVector CurrentLocation, FVector InputTargetLocation, FRotator InputTargetRotator );

	UFUNCTION(BlueprintCallable)
		float GetEmitterCurveValue(FName CurveName);


public:	
	// Sets default values for this actor's properties
	AHEmitter();

	// OnEvent
protected:

	//@TODO: also should add Action Container TagCheck.
	void OnTriggerPressed();
	void OnTriggerReleased();

	void OnAimingstateChanged();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	inline bool IsBackward() const { return bIsAim; };
	inline float GetAimingDistance () const { return AimingDistance; };
	inline USkeletalMeshComponent* GetMesh() const { return ComEmitterMesh; };

	inline UHEmitterSettings* GetESPMSettings() const { return ESPMBaseSetting; };

	//偷懒
	friend class UHEmitterAnim;


};

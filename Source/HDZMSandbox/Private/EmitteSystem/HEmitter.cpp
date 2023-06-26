// Fill out your copyright notice in the Description page of Project Settings.


#include "EmitteSystem/HEmitter.h"
#include "Components/ArrowComponent.h"
#include <HEmitterComponent.h>
#include "Components/SkeletalMeshComponent.h"
#include "EmitteSystem/HEmitterAnim.h"
#include "HPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"

void AHEmitter::UpdateEssentialValues()
{
	if (!(OwnerPawn && OwnerController))
		return;

	ShootType = OwnerPawn->GetShootType();
	bIsAim = OwnerPawn->GetIsEmitterAiming();

	InputAcceleration = OwnerPawn->GetCharacterMovement()->GetCurrentAcceleration();
	bHasMovementInput = OwnerPawn->GetLocomotionState().bHasInput;


	// PlayerCameraMange 能够接受AlsCameraComponent的相机数据. 可以使用.
	CameraLocation = OwnerController->PlayerCameraManager->GetCameraLocation();
	CameraRotation = OwnerController->PlayerCameraManager->GetCameraRotation();

	CharacterRotator = OwnerPawn->GetActorRotation();

}



void AHEmitter::UpdateEmitterBehaviour()
{
	FTransform  PivotTransform = OwnerPawn->GetActorTransform();
	float DesiredDistance = GetEmitterCurveValue(Curve_DistanceToPivot);

	SmoothPivotLocation = CaculateAxisIndepentLag(SmoothPivotLocation, PivotTransform.GetLocation(), PivotTransform.Rotator());
	 FVector SmoothedTargetPivotLocation = SmoothPivotLocation + UKismetMathLibrary::GetForwardVector(PivotTransform.Rotator()) * GetEmitterCurveValue(Curve_PivotXOffset)
		+ UKismetMathLibrary::GetUpVector(PivotTransform.Rotator()) * GetEmitterCurveValue(Curve_PivotZOffset)
		+ UKismetMathLibrary::GetRightVector(PivotTransform.Rotator()) * GetEmitterCurveValue(Curve_PivotYOffset);
	
	//当瞄准时使用controller的Rotator, 避免因为视线击中物体的不同导致不稳定变化.
	 if (!bIsAim) UpdateAimRotator(SmoothedTargetPivotLocation);
	else AimingRotator = OwnerController->GetControlRotation();

	//Emitter的期望旋转
	if (bIsAim)
	{
		if (bHasMovementInput)
		{
			TargetRotator = GetAimBackRotatorWithMoveInputFac();
		}
		else
		{ 
			TargetRotator =  GetAimBackRotatorWithControlFac();
		}
		
	}
	else TargetRotator = AimingRotator;

	SmoothedRotator = UKismetMathLibrary::RInterpTo(SmoothedRotator, TargetRotator, DeltaSeconds, GetEmitterCurveValue(Curve_RotatorSpeed));
	

	//Radius length & radius Trace
	FVector endP{ SmoothedTargetPivotLocation + UKismetMathLibrary::GetForwardVector(SmoothedRotator) * DesiredDistance };
	const auto CollisionShape{ FCollisionShape::MakeSphere(EmitterRadius * FinalScaleFac) };

	const float AnimScaledFac{ GetEmitterCurveValue(Curve_ScaledFac) };

	FHitResult hitresult;
// 	FCollisionQueryParams collisionParameters;
// 
// 	collisionParameters.AddIgnoredActor(OwnerPawn);


// 	// 起始点和检测结果
// FVector BeginLoc = GetActorLocation();
// FVector EndLoc = BeginLoc + GetActorForwardVector() * 1000;
// FHitResult HitResult;
//  
// // 要忽略的物体
// TArray<AActor*> IgnoreActors;	
// 	
// 	// 射线检测
// bool bIsHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), BeginLoc, EndLoc, TraceTypeQuery1, false, IgnoreActors, EDrawDebugTrace::ForDuration, HitResult, true);
// if (bIsHit)
// {
// 	UKismetSystemLibrary::PrintString(GetWorld(), HitResult.GetActor()->GetName());
// }


	TArray<AActor*> IgnoreActors{OwnerPawn};



	if (UKismetSystemLibrary::SphereTraceSingle(GetWorld(), SmoothedTargetPivotLocation, endP, EmitterRadius*AnimScaledFac, TraceTypeQuery1, false, IgnoreActors,
		EDrawDebugTrace::ForOneFrame, hitresult, true) )
	{
		TargetDistance = hitresult.Distance > 40.f ? hitresult.Distance - 10.f : 30.f;
		FinalScaleFac  = FMath::Lerp(0.5, 1.0, hitresult.Time) * AnimScaledFac;
	}
	else
	{
		TargetDistance = DesiredDistance;
		FinalScaleFac = AnimScaledFac;
	}

	//应用.
	CurDistance = FMath::FInterpTo(CurDistance,TargetDistance,DeltaSeconds, AdaptDistSpeed);
	const auto SmoothedLocation{ UKismetMathLibrary::GetForwardVector(SmoothedRotator) * CurDistance + SmoothedTargetPivotLocation };


	SetActorLocationAndRotation(SmoothedLocation, SmoothedRotator);

	DrawDebugBox(GetWorld(), SmoothedLocation, FVector( 20.f,20.f,20.f ), FQuat::Identity, FColor::Red,false,0.f);
	DrawDebugSphere(GetWorld(), SmoothedTargetPivotLocation,20.f,16,FColor::Red,false,0.f);


}

void AHEmitter::UpdateAimRotator(FVector CurrentTargetPivotLocation)
{
	FVector StartP = CameraLocation + UKismetMathLibrary::GetForwardVector(CameraRotation) * 10;
	FVector endP = CameraLocation + UKismetMathLibrary::GetForwardVector(CameraRotation) * 10000;

	FHitResult hitresult;
	FCollisionQueryParams queryParams;
	queryParams.AddIgnoredActor(OwnerPawn);
	queryParams.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(hitresult, StartP, endP, ECC_Visibility, queryParams) && hitresult.IsValidBlockingHit())
	{
		endP = hitresult.Time>0.5f ? hitresult.Location : CameraLocation + UKismetMathLibrary::GetForwardVector(CameraRotation) * 5000;
	};
	AimingRotator = UKismetMathLibrary::MakeRotFromX(endP - CurrentTargetPivotLocation);
}

 
void AHEmitter::InitInputBind()
{
	InputComponent->BindAxis("AdjustPitch",this,&AHEmitter::OnWheelSlide);
	InputComponent->BindAction("Trigger",EInputEvent::IE_Pressed,this,&AHEmitter::OnTriggerPressed);
	InputComponent->BindAction("Trigger",EInputEvent::IE_Released,this,&AHEmitter::OnTriggerReleased);


}

void AHEmitter::OnWheelSlide(float val)
{
	if (!bIsAim) return;
	EnteringAimingPitch += PitchSpeed * val;

}

inline FRotator AHEmitter::GetAimBackRotatorWithMoveInputFac()
{
	const auto InputRot{ UKismetMathLibrary::NormalizedDeltaRotator(UKismetMathLibrary::MakeRotFromX(InputAcceleration),CharacterRotator) };

	const float tempYaw{
		FMath::Clamp(
			InputRot.Yaw + FMath::GetMappedRangeValueClamped(
					{ -90.f,90.f }, { -15.f,15.f }, UKismetMathLibrary::NormalizedDeltaRotator(AimingRotator, InputRot).Yaw
				)+180.f
			,80.f,290.f)
		 };
	return FRotator(EnteringAimingPitch, ((tempYaw > 275.f || tempYaw < 85.f) ? 180.f : tempYaw) + CharacterRotator.Yaw ,CharacterRotator.Roll );

	//@Attention: FRotator的构造顺序是Pitch Yaw Roll. 和蓝图的roll pitch yaw 不一样.
}

inline FRotator AHEmitter::GetAimBackRotatorWithControlFac()
{
	const auto tempYaw{
	FMath::GetMappedRangeValueClamped({ -90.f,90.f }, { -60.f,60.f}, UKismetMathLibrary::NormalizedDeltaRotator(AimingRotator,CharacterRotator).Yaw)
	+ 180.f + CharacterRotator.Yaw
	};
	return  FRotator(EnteringAimingPitch, tempYaw, CharacterRotator.Roll);
}

inline FVector AHEmitter::CaculateAxisIndepentLag(FVector CurrentLocation, FVector TargetLocation, FRotator InputTargetRotator, FVector LagSpeed /*= FVector(14,14,16) */)
{
	FRotator TargetRotatroYaw{ 0.f,0.f,InputTargetRotator.Yaw };

	
	FVector UnRotLocation = TargetRotatroYaw.UnrotateVector(CurrentLocation);
	FVector UnRotTargetLocation = TargetRotatroYaw.UnrotateVector(TargetLocation);

	return  TargetRotatroYaw.RotateVector(
		FVector{
		UKismetMathLibrary::FInterpTo(UnRotLocation.X,UnRotTargetLocation.X,DeltaSeconds,LagSpeed.X),
		UKismetMathLibrary::FInterpTo(UnRotLocation.Y,UnRotTargetLocation.Y,DeltaSeconds,LagSpeed.Y),
		UKismetMathLibrary::FInterpTo(UnRotLocation.Z,UnRotTargetLocation.Z,DeltaSeconds,LagSpeed.Z)
		}
	);
}

inline float AHEmitter::GetEmitterCurveValue(FName CurveName)
{
	return ComEmitterMesh->GetAnimInstance()->GetCurveValue(CurveName);
}

// Sets default values
AHEmitter::AHEmitter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ComEmitterMesh = CreateDefaultSubobject<USkeletalMeshComponent>("EmitterSKMesh");
	ComEmitterMesh->SetupAttachment(GetRootComponent());

	ComEmitterDirection = CreateDefaultSubobject<UArrowComponent>("EmitterDirection");
	ComEmitterDirection->SetupAttachment(ComEmitterMesh);

	ComHEmitter = CreateDefaultSubobject<UHEmitterComponent>("EmitterFunc");

}

void AHEmitter::OnTriggerPressed()
{
	ComHEmitter->OnTrigerPressed(GetInstigator());

}

void AHEmitter::OnTriggerReleased()
{

	ComHEmitter->OnTrigerReleased(GetInstigator());
}

void AHEmitter::OnAimingstateChanged()
{
	ComHEmitter->OnSwitchAimingState();

}

// Called when the game starts or when spawned
void AHEmitter::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerPawn = Cast<AHPlayerCharacter>(GetOwner());
	if (ensure(OwnerPawn))
	{
		OwnerController = Cast<APlayerController>(OwnerPawn->GetController());
		EnableInput(OwnerController); //接受输入/输入会传播给Emitter;
	};

	//EnableInput(OwnerController); //接受输入/输入会传播给Emitter;
	InitInputBind();


}

void AHEmitter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

// Called every frame
void AHEmitter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DeltaSeconds = DeltaTime;

	if (!OwnerPawn) return;
	
	UpdateEssentialValues();

	

	if (bIsAim != bPreIsAim) //Changed to true;
	{
		OnAimingstateChanged();
		if (bIsAim)
		{
			EnteringAimingPitch = GetActorRotation().Pitch * -1.f;

		}

	}

	bPreIsAim = bIsAim; //Update after check;
	
	UpdateEmitterBehaviour();

}


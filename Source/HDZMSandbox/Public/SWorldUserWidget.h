// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SWorldUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class HDZMSANDBOX_API USWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()

/// <summary>
/// worldUserWight
/// @Describe: project widget to an actor camera
/// @other: for use widgetLibrary , your uporject should add modlue UMG (.build.cs also should add)
/// </summary>

public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


	//control size;
	UPROPERTY(meta = (BindWidget))
	class USizeBox* ParentSizeBox;
	//@attention: Palette bind to it by changing name


	UPROPERTY(BlueprintReadWrite, Category = "UI", meta = (ExposeOnSpawn = true))
	AActor* AttachedActor;
	//owning actor
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "UI")
	FVector Worldoffset;


	//virtual void NativeOnInitialized();
	//virtual void NativePreConstruct();
	//virtual void NativeConstruct();
	//virtual void NativeDestruct();
	//
	//virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
	//
	//UE_DEPRECATED(4.20, "Please override the other version of NativePaint that accepts all the parameters, not just the paint context.")
	//	virtual void NativePaint(FPaintContext& InContext) const { }

};

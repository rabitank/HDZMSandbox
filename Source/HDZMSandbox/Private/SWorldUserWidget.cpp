// Fill out your copyright notice in the Description page of Project Settings.


#include "SWorldUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"

void USWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	FVector2D screenPosition;
	if (!IsValid(AttachedActor))
	{
		//after attachedActor died and be restored by UE (60s/per)
		//*AttachedActor is null
		//before restored, AttachedActor become being pendingkilled() (SetLifeSpan)

		RemoveFromParent();
		UE_LOG(LogTemp,Warning,TEXT("Attached actor no longer valid,removing health widget"))
		return;
	}
	if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation()+Worldoffset, screenPosition))
	{
		float viewportScale = UWidgetLayoutLibrary::GetViewportScale(this);
		screenPosition /= viewportScale;

		//about sizebox: view:https://www.bbsmax.com/A/gVdnZreXdW/
		//一、Size Box用来指定一个特定的尺寸
		//二、Size Box只能放一个子控件
		//三、Size Box一般作为Canvas Panel的子控件，并勾选Size To Content选项，而不作为根节点。

		if (ParentSizeBox)
		{
			ParentSizeBox->SetRenderTranslation(screenPosition);
		}

	};



}

// Fill out your copyright notice in the Description page of Project Settings.


#include "LineSTG/LineSTGBlueprintFunctionLibrary.h"
#include "Components/TimelineComponent.h"



void ULineSTGBlueprintFunctionLibrary::UpdateTimelineComp_MoveToIn(float Output)
{

}

void ULineSTGBlueprintFunctionLibrary::MoveToIn(AActor* actor, FVector location, float inseconds, LineSTG::LerpMode lerpMode)
{

// 	FOnTimelineFloat UpdateFunctionFloat;
// 
// 	//将浮点轨道绑定到UpdateTimelineComp函数的输出
// 	UpdateFunctionFloat.BindDynamic(,);
// 
// 	UTimelineComponent* moveTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("TempTimeline"));
// 	
// 	//将其图表绑定到我们的更新函数
// 	switch (lerpMode)
// 	{
// 	case LineSTG::Linear:
// 		moveTimeline->AddInterpFloat(, UpdateFunctionFloat);
// 		break;
// 
// 	case LineSTG::Smooth:
// 		moveTimeline->AddInterpFloat(, UpdateFunctionFloat);
// 		break;
// 	};
// 
// 
// 
// 	
// 	moveTimeline->Play();




}

AHBulletBase* ULineSTGBlueprintFunctionLibrary::CreatBullet(FName bulletName, FVector positon, FRotator rot, APawn* Initigator)
{

	return nullptr;
}

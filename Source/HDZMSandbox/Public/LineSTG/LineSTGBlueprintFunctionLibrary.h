// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HBulletBase.h"
#include "LineSTGBlueprintFunctionLibrary.generated.h"

/**
 * 
 */

UENUM()
namespace LineSTG
{
	enum LerpMode
	{
		Linear,
		Smooth,
	};
}


UCLASS()
class HDZMSANDBOX_API ULineSTGBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
private:
	//���ڴ������ǵĸ��¹���¼��ĸ�����ǩ��
	

	//����ʹ��ʱ����ͼ����������λ�õĺ���
	UFUNCTION()
	static void UpdateTimelineComp_MoveToIn(float Output);

protected:
	

public:
	static void MoveToIn(AActor* actor, FVector location, float inseconds, LineSTG::LerpMode lerpMode);
	static AHBulletBase* CreatBullet(FName bulletName, FVector positon, FRotator rot,APawn* Initigator);


};

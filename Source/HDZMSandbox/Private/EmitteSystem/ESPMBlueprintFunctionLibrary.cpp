// Fill out your copyright notice in the Description page of Project Settings.


#include "EmitteSystem/ESPMBlueprintFunctionLibrary.h"
#include "HPlayerCharacter.h"
#include "EmitteSystem/HEmitter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EmitteSystem/HEmitterSettings.h"
#include <HAttributeComponent.h>
#include "EmitteSystem/HEmitterPattern.h"
#include "EmitteSystem/HEnergyComponent.h"


bool UESPMBlueprintFunctionLibrary::AddRecoilByEmittePattern(FName emittePatternName, AHEmitter* emitter, AHPlayerCharacter* target)
{
	const UHEmitterSettings* setting =  emitter->GetESPMSettings();

	//����������򷵻����͵�Ĭ��ֵ/ 0;
	const float patternRecoil{ setting->GetPatternRecoil(emittePatternName ) };

	UCharacterMovementComponent* movecomp = target->GetCharacterMovement();
	auto rot{emitter->GetActorRotation().Vector()};
	
	
	movecomp->AddImpulse(-rot*patternRecoil, true);
	

	return true;
}



bool UESPMBlueprintFunctionLibrary::HasEnoughOneShootEnergy(AHEmitterPattern* emittePattern, AHEmitter* emitter, AActor* target)
{

	UHEnergyComponent* attribute = UHEnergyComponent::GetAttribute(emitter);
	if (!ensure(attribute)) return false;

	const UHEmitterSettings* setting = emitter->GetESPMSettings();
	//����������򷵻����͵�Ĭ��ֵ/ 0;
	const float patternEngergy{ setting->GetPatternBaseEnergy(emittePattern->GetPatternName() ) };

	return attribute->IsAffordable(patternEngergy);

}

bool UESPMBlueprintFunctionLibrary::ApplyEmittePatternEnergy(AHEmitterPattern* emittePattern, AHEmitter* emitter, AActor* target)
{
		UHEnergyComponent* targetAttribute = UHEnergyComponent::GetAttribute(emitter);
		if (targetAttribute)
		{
			const UHEmitterSettings* setting = emitter->GetESPMSettings();


			//����������򷵻����͵�Ĭ��ֵ/ 0; ������Ч�ǵ����������ֱ�ע
			//�õ�patternEnergy�����̻��.
			const float patternEngergy{ setting->GetPatternBaseEnergy(emittePattern->GetPatternName()) };

			//����Ϊ��emitter����
			return targetAttribute->ApplyEnergyChangeDelta(emitter, patternEngergy);
		}

		return false;
}


/// <summary>
/// TargetActor: should has energycomp
/// </summary>
/// <param name="CauserActor"></param>
/// <param name="TargetActor"></param>
/// <param name="DamageAmount"></param>
/// <returns></returns>
bool UESPMBlueprintFunctionLibrary::ApplyEnergy(AActor* CauserActor, AActor* TargetActor, float DamageAmount)
{
	UHEnergyComponent* targetAttribute = UHEnergyComponent::GetAttribute(TargetActor);
	if (targetAttribute)
	{
		return targetAttribute->ApplyEnergyChangeDelta(CauserActor, DamageAmount);
	}
	return false;
}


/// <summary>
/// ������,TargetActorӦ����energycomp.
/// </summary>
/// <param name="CauserActor"></param>
/// <param name="TargetActor"></param>
/// <param name="EnergyAmount"></param>
/// <param name="hit"></param>
/// <returns></returns>
bool UESPMBlueprintFunctionLibrary::ApplyDirectionalEnergy(AActor* CauserActor, AActor* TargetActor, float EnergyAmount, const FHitResult& hit)
{
	if (ApplyEnergy(CauserActor, TargetActor, EnergyAmount))
	{
		UPrimitiveComponent* comHit = hit.GetComponent();
// 		if (comHit && comHit->IsSimulatingPhysics(hit.BoneName))
// 		{
// 			comHit->AddImpulseAtLocation(-hit.ImpactNormal * 30000.f, hit.ImpactPoint, hit.BoneName);
// 		}

		return true;
	}
	return false;


}

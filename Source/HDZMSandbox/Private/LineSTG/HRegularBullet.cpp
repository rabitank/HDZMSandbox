// Fill out your copyright notice in the Description page of Project Settings.


#include "LineSTG/HRegularBullet.h"

void AHRegularBullet::UpdateVelcoity(float DeltaTime)
{

	LinearVelocity = FMath::Clamp(LinearVelocity + LinearAcceleration * DeltaTime, -MaxLinearVeclocity, MaxLinearVeclocity);
	AngularVelocity += AngularAcceleration * DeltaTime;
}

void AHRegularBullet::Drive_Implementation(float deltaTime)
{
	Super::Drive_Implementation(deltaTime);

	///@TODO:Update() 切换为注入模式. 使用可配置的调用对象来更换行为模式.
	UpdateAccelreation(deltaTime);
	UpdateVelcoity(deltaTime);

	AddActorLocalOffset(FVector::ForwardVector *LinearVelocity * deltaTime, false);
	AddActorLocalRotation(FQuat(FVector::UpVector,AngularVelocity*deltaTime),false);
}

void AHRegularBullet::Init_Implementation(AHBulletBase* initorB)
{
	Super::Init_Implementation(initorB);
	const auto initor = Cast<AHRegularBullet>(initorB);
	LinearVelocity =  initor->LinearVelocity;
	LinearAcceleration = initor->LinearAcceleration;
	MaxLinearVeclocity = initor->MaxLinearVeclocity;
	AngularVelocity = initor->AngularAcceleration;
	AngularAxis =initor->AngularAxis;
	AngularAcceleration = initor->AngularAcceleration;
}

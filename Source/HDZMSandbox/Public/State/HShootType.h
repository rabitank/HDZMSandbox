// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "HShootType.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EHShootType : uint8
{
	None,
	ForwardShoot,
	BackwardShoot
};

USTRUCT(BlueprintType)
struct HDZMSANDBOX_API FHShootType
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
		EHShootType ShooType {EHShootType::None};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
		bool bForwardShoot{false};
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
		bool bBackwardShoot{false};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
		bool bShooting{false};

public:
	FHShootType() = default;
	FHShootType(const EHShootType InitialShootType)
	{
		*this = InitialShootType;

	};

	bool IsbShooting() const
	{
		return bShooting;
	}
	bool IsForwardShooting() const
	{
		return bForwardShoot;

	}
	bool IsBackwardShooting() const
	{
		return bBackwardShoot;
	}

	void operator=(const EHShootType NewShootType)
	{
		ShooType = NewShootType;
		bForwardShoot = ShooType == EHShootType::ForwardShoot ? true : false;
		bBackwardShoot = ShooType == EHShootType::BackwardShoot ? true : false;
		bShooting = ShooType == EHShootType::None ? false : true;
	}

};

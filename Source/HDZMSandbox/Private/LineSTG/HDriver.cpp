#include "LineSTG/HDriver.h"
#include "LineSTG/HComplexBullet.h"

unsigned int UHDriver::CreatedCount  = 0;

UHDriver* UHDriver::CloneToAttach_Implementation(AHComplexBullet* beDrivinger)
{
	//or  ->getClass().ischildof(staticClass);
	const bool result = beDrivinger->IsA(OwningerClass);
	if(ensureMsgf(result,TEXT("Dirvers'class is not match with bullet")))
	{
		UHDriver* cloneD  = CloneOne();
		cloneD->InitAttachment(beDrivinger);
		return cloneD;
	}
	return nullptr;
	
}

void UHDriver::DriverUpdate_Implementation(float DeltaTime)
{
	//make sure owninger is vaild;
	//wait to write in BP;
	//Must Init In BP.
	ensure(Owninger);
}

UHDriver::UHDriver()
{
	OwningerClass= AHComplexBullet::StaticClass();
}

bool UHDriver::ResetAttachment_Implementation(AHComplexBullet* attachment)
{
	//or  ->getClass().ischildof(staticClass);
	const bool result = attachment->IsA(OwningerClass->StaticClass());
	if(ensureMsgf(result,TEXT("Dirvers'class is not match with bullet")))
	{
		Owninger= attachment;
	}
	return result;
}

//MustOverride 在叶子类中必须复写!!!! -> 向新的obj传递
UHDriver* UHDriver::CloneOne_Implementation()
{
	CreatedCount++;
	UE_LOG(LogTemp,Warning,TEXT("%s cloned"),*GetName())
	auto const newname = FString::Printf(TEXT("Driver_%s_%d"),*GetClass()->GetName(),CreatedCount);
	UHDriver* a = NewObject<UHDriver>(GetOuter(),GetClass(),*newname); //get this ins's actual class
	return  a;
}

void UHDriver::InitAttachment_Implementation(AHComplexBullet* attachment)
{
	Owninger = attachment;
}

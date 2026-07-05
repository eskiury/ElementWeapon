#include "WeaponTriggerComponent.h"

#include "../WeaponBase.h"

UWeaponTriggerComponent::UWeaponTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

void UWeaponTriggerComponent::InitializeComponentContext(AWeaponBase* Weapon)
{
	MyWeaponOwner = Weapon;
}

void UWeaponTriggerComponent::PullTrigger()
{
	if(!GetWorld()->GetTimerManager().IsTimerActive(StreamTimerHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(StreamTimerHandle, this, &UWeaponTriggerComponent::FireShot, FireRate, true, -1.0f);
	}
}

void UWeaponTriggerComponent::ReleaseTrigger()
{
	GetWorld()->GetTimerManager().ClearTimer(StreamTimerHandle);
}

void UWeaponTriggerComponent::FireShot()
{
	if (MyWeaponOwner != NULL && MyWeaponOwner->GetCurrentBarrel() != NULL)
	{
		MyWeaponOwner->ShotBarrel();
	}
}



#include "WeaponTriggerComponent.h"

#include "../Standard/WeaponBase.h"
#include "../../Elemental/ElementalDataAsset.h"

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
	if (InfusedElement != nullptr && InfusedElement->FireModeAction != nullptr)
	{
		InfusedElement->FireModeAction->ExecuteFireMode(this);
	}
	else
	{
		if (!bHasFiredThisPull)
		{
			FireShot();
			bHasFiredThisPull = true;
		}

	}
}

void UWeaponTriggerComponent::ReleaseTrigger()
{
	GetWorld()->GetTimerManager().ClearTimer(StreamTimerHandle);

	bHasFiredThisPull = false;
}

void UWeaponTriggerComponent::FireShot()
{
	if (MyWeaponOwner != NULL && MyWeaponOwner->GetCurrentBarrel() != NULL)
	{
		MyWeaponOwner->ShotBarrel();
	}
}



#include "WeaponTriggerComponent.h"

#include "../WeaponBase.h"
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
	if (InfusedElement != nullptr && InfusedElement->TriggerAction != nullptr)
	{
		InfusedElement->TriggerAction->ExecuteTriggerModifier(this);
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



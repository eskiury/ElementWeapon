#include "WeaponTriggerComponent.h"

UWeaponTriggerComponent::UWeaponTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWeaponTriggerComponent::InitializeComponentContext(AWeaponBase* Weapon)
{
	MyWeaponOwner = Weapon;
}
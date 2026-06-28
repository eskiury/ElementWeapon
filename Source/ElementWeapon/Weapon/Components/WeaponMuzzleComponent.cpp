#include "WeaponMuzzleComponent.h"

UWeaponMuzzleComponent::UWeaponMuzzleComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWeaponMuzzleComponent::InitializeComponentContext(AWeaponBase* Weapon)
{
	MyWeaponOwner = Weapon;
}
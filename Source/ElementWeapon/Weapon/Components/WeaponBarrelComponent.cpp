#include "WeaponBarrelComponent.h"

#include "../../Elemental/ElementalDataAsset.h"

UWeaponBarrelComponent::UWeaponBarrelComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	//Configuracion del Spawner de balas
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

}

void UWeaponBarrelComponent::InitializeComponentContext(AWeaponBase* Weapon)
{
	MyWeaponOwner = Weapon;
}

void UWeaponBarrelComponent::FireProjectile()
{
	//Check elemento seleccionado
	if (InfusedElement != nullptr)
	{
		//Check elemento tiene accion especifica
		if (InfusedElement->BarrelAction != nullptr)
		{
			InfusedElement->BarrelAction->ExecuteBarrelModifier(this);
		}
	}

	FTransform Transform = GetOwner()->GetActorTransform();
	GetWorld()->SpawnActor<AActor>(ProjectileClass, Transform, SpawnParams);
}

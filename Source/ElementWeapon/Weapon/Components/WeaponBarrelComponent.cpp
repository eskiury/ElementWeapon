#include "WeaponBarrelComponent.h"

#include "../../Elemental/ElementalDataAsset.h"
#include "../../Weapon/WeaponProjectile.h"
#include "../WeaponBase.h"

UWeaponBarrelComponent::UWeaponBarrelComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	//Configuracion del Spawner de balas
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Owner = GetOwner();

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
}

void UWeaponBarrelComponent::SpawnSingleActor(FVector Location, FRotator Rotation) const
{
	if (ProjectileClass == nullptr) return;

	//Spawneamos al actor (Projectile) en la posicion del arma sin colisionar unas con otras y con la rotacion alterada por el Modifier
	AWeaponProjectile* SpawnedActor = GetWorld()->SpawnActor<AWeaponProjectile>(ProjectileClass, Location, Rotation, SpawnParams);

	if (SpawnedActor)
	{
		// ¡Éxito! El actor se ha spawneado correctamente.
		MyWeaponOwner->ShotMuzzle(SpawnedActor);
	}
	else
	{
		// El spawn ha fallado
		UE_LOG(LogTemp, Warning, TEXT("Error: No se pudo spawnear el Actor."));
	}

}

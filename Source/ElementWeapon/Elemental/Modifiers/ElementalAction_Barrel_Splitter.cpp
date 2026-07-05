#include "ElementalAction_Barrel_Splitter.h"

#include "../../Weapon/Components/WeaponBarrelComponent.h"

void UElementalAction_Barrel_Splitter::ExecuteBarrelModifier(UWeaponBarrelComponent* BarrelComponent) const
{
	if (BarrelComponent == nullptr || BarrelComponent->GetOwner() == nullptr) return;

	FVector Location = BarrelComponent->GetOwner()->GetActorLocation();
	FRotator Rotacion = BarrelComponent->GetOwner()->GetActorRotation();

	//Enviamos el comando de Spawnear al WeaponBarrelComponent con la rotacion alterada y un numero X de veces.
	for (int i = 0; i < ProjectileCount; i++)
	{			
		//Rotacion + FRotator(FMath::RandRange(-SpreadAngle, SpreadAngle), FMath::RandRange(-SpreadAngle, SpreadAngle), 0.0f)
		//Suma del Rotator Main, mas los dos Randoms, pero ahorramos memoria que esto se repite mucho.
		BarrelComponent->SpawnSingleActor(Location, Rotacion + FRotator(FMath::RandRange(-SpreadAngle, SpreadAngle), FMath::RandRange(-SpreadAngle, SpreadAngle), 0.0f));
	}
}

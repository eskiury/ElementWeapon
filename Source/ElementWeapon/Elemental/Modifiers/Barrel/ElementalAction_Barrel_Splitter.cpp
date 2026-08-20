#include "ElementalAction_Barrel_Splitter.h"

#include "../../../Weapon/Components/WeaponBarrelComponent.h"

void UElementalAction_Barrel_Splitter::ExecuteBarrelModifier(UWeaponBarrelComponent* Barrel, const FVector& BaseLocation, const FRotator& BaseRotation) const
{
	if (!Barrel) return;

	// Ejemplo de escopeta / división:
	// Mantenemos BaseLocation intacto (siempre la punta del Muzzle)
	// y alteramos únicamente la rotación para cada proyectil
	for (int32 i = 0; i < NumberOfPellets; ++i)
	{
		FRotator SpreadRotation = BaseRotation;
		SpreadRotation.Yaw += FMath::RandRange(-SpreadAngle, SpreadAngle);
		SpreadRotation.Pitch += FMath::RandRange(-SpreadAngle, SpreadAngle);

		Barrel->DeliverShot(BaseLocation, SpreadRotation);
	}
}

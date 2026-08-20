#include "ElementalAction_Barrel_Arco.h"

#include "../../../Weapon/Components/WeaponBarrelComponent.h"

void UElementalAction_Barrel_Arco::ExecuteBarrelModifier(class UWeaponBarrelComponent* BarrelComponent, const FVector& BaseLocation, const FRotator& BaseRotation) const
{
	if (BarrelComponent == nullptr || BarrelComponent->GetOwner() == nullptr) return;

	APawn* PlayerPawn = Cast<APawn>(BarrelComponent->GetOwner()->GetOwner());

	float paso = 0.0f;
	if (ProjectileCount > 1)
	{
		paso = SpreadAngle / (ProjectileCount - 1); // 30.0 / 9 = 3.333º
	}
	double Adjustment = -(SpreadAngle / 2);
	double Yaw = 0.0f;
	//Enviamos el comando de Spawnear al WeaponBarrelComponent con la rotacion alterada y un numero X de veces.
	for (int i = 0; i < ProjectileCount; ++i)
	{
		Yaw = (i * paso) + Adjustment;
		BarrelComponent->DeliverShot(BaseLocation, BaseRotation + FRotator(0.0f, Yaw, 0.0f));

	}
}

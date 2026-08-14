#include "ElementalAction_Muzzle.h"
#include "../../Hud/DamageNumberActor.h"

void UElementalAction_Muzzle::ShowDamageNumbers(const FVector& SpawnLocation, const float& TotalDamage, const FLinearColor& Color, UWorld* World) const
{
	if (DamageActorClass)
	{
		ADamageNumberActor* DamageActor = World->SpawnActor<ADamageNumberActor>(
			DamageActorClass,
			SpawnLocation,
			FRotator::ZeroRotator
		);

		if (DamageActor)
		{
			DamageActor->SetDamageNumber(TotalDamage, Color);
		}
	}
	return;
}

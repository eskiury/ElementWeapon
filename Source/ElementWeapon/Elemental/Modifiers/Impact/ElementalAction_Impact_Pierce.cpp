#include "ElementalAction_Impact_Pierce.h"
#include "../../../Weapon/Standard/StatusComponent.h" // ◄ OBLIGATORIO para FindComponentByClass

TSet<AActor*> UElementalAction_Impact_Pierce::ExecuteImpactModifier(
	const FHitResult& HitResult,
	const UWorld* World,
	bool& bOutShouldDestroy) const
{
	TSet<AActor*> ProcessedActors;
	AActor* HitActor = HitResult.GetActor();

	if (HitActor)
	{
		ProcessedActors.Add(HitActor);
	}

	// Si choca contra una pared o algo sin vida, la bala debe destruirse
	if (HitActor == nullptr || HitActor->FindComponentByClass<UStatusComponent>() == nullptr)
	{
		bOutShouldDestroy = true;
	}
	else
	{
		// Si es un enemigo válido, permitimos que continúe viva para atravesarlo
		bOutShouldDestroy = false;
	}

	return ProcessedActors;
}
#include "ElementalAction_Muzzle_Burn.h"

void UElementalAction_Muzzle_Burn::ExecuteMuzzleModifier(AActor* TargetActor, const FHitResult& HitResult) const
{
	if (TargetActor == nullptr) return;

	UStatusComponent* StatusComp = TargetActor->FindComponentByClass<UStatusComponent>();
	if (StatusComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("Añadido elemento fuego a Actor"));

	}
}
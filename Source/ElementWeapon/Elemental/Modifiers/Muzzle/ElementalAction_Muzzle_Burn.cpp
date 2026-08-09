#include "ElementalAction_Muzzle_Burn.h"

#include "../../../Weapon/Standard/StatusComponent.h"

void UElementalAction_Muzzle_Burn::ExecuteMuzzleModifier(AActor* TargetActor, const FHitResult& HitResult) const
{
	if (TargetActor == nullptr) return;

	UStatusComponent* StatusComp = TargetActor->FindComponentByClass<UStatusComponent>();
	if (StatusComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("Añadido elemento fuego a Actor"));
		StatusComp->ApplyStatusEffect(GetClass(), Duration, TickInterval);
	}
}

void UElementalAction_Muzzle_Burn::OnStatusTick(UStatusComponent* TargetComp, FActiveStatusEffect& EffectData) const
{
	if (EffectData.StackCount == 0)
	{
		TargetComp->SetHealth(TargetComp->GetHealth() - Damage);
	}
	else
	{
		TargetComp->SetHealth(TargetComp->GetHealth() - (Damage + EffectData.StackCount));
	}
	return;
}

void UElementalAction_Muzzle_Burn::OnStatusExpired(UStatusComponent* TargetComp, const FActiveStatusEffect& EffectData) const
{
	return;
}

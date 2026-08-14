#include "ElementalAction_Muzzle_Burn.h"

#include "../../../Weapon/Standard/StatusComponent.h"

void UElementalAction_Muzzle_Burn::ExecuteMuzzleModifier(AActor* TargetActor, const FHitResult& HitResult) const
{
	if (TargetActor == nullptr) return;

	UStatusComponent* StatusComp = TargetActor->FindComponentByClass<UStatusComponent>();
	if (StatusComp)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Añadido elemento fuego a Actor"));
		StatusComp->ApplyStatusEffect(GetClass(), Duration, TickInterval);

	}
}

void UElementalAction_Muzzle_Burn::OnStatusTick(UStatusComponent* TargetComp, FActiveStatusEffect& EffectData) const
{
	float TotalDamage = Damage;

	if (EffectData.StackCount > 0) TotalDamage += EffectData.StackCount;

	TargetComp->SetHealth(TargetComp->GetHealth() - TotalDamage);

	FVector SpawnLocation = TargetComp->GetOwner()->GetActorLocation() + FVector(0.0f, 0.0f, 90.0f);
	ShowDamageNumbers(SpawnLocation, TotalDamage, ElementColor, TargetComp->GetWorld());
	return;
}

void UElementalAction_Muzzle_Burn::OnStatusExpired(UStatusComponent* TargetComp, const FActiveStatusEffect& EffectData) const
{
	
	return;
}

void UElementalAction_Muzzle_Burn::OnStatusHitted(UStatusComponent* TargetComp, FActiveStatusEffect& EffectData) const
{
	EffectData.RemainingDuration = Duration;	//Reinicia contador de effecto

	float TotalDamage = Damage;

	if (EffectData.StackCount > 0) TotalDamage += EffectData.StackCount;

	TargetComp->SetHealth(TargetComp->GetHealth() - TotalDamage);

	FVector SpawnLocation = TargetComp->GetOwner()->GetActorLocation() + FVector(0.0f, 0.0f, 90.0f);
	ShowDamageNumbers(SpawnLocation, TotalDamage, ElementColor, TargetComp->GetWorld());
}

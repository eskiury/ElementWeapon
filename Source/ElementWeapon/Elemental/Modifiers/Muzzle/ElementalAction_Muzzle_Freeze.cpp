#include "ElementalAction_Muzzle_Freeze.h"

#include "../../../Weapon/Standard/StatusComponent.h"

void UElementalAction_Muzzle_Freeze::ExecuteMuzzleModifier(AActor* TargetActor, const FHitResult& HitResult) const
{
	if (TargetActor == nullptr) return;

	UStatusComponent* StatusComp = TargetActor->FindComponentByClass<UStatusComponent>();
	if (StatusComp)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Añadido elemento hielo a Actor"));
		StatusComp->ApplyStatusEffect(GetClass(), Duration, TickInterval);

	}
}

void UElementalAction_Muzzle_Freeze::OnStatusTick(UStatusComponent* TargetComp, FActiveStatusEffect& EffectData) const
{		
	TargetComp->SetHealth(TargetComp->GetHealth() - Damage);

	FVector SpawnLocation = TargetComp->GetOwner()->GetActorLocation() + FVector(0.0f, 0.0f, 90.0f);
	ShowDamageNumbers(SpawnLocation, Damage, ElementColor, TargetComp->GetWorld());
	return;
}

void UElementalAction_Muzzle_Freeze::OnStatusExpired(UStatusComponent* TargetComp, const FActiveStatusEffect& EffectData) const
{
	TargetComp->SetSpeed(TargetComp->GetMaxSpeed());
	TargetComp->SetArmor(1.0f); //Esto dara fallo si meto otro cosa relacionada porque reseteara sin importar si hay otro elemento afectandole
	return;
}

void UElementalAction_Muzzle_Freeze::OnStatusHitted(UStatusComponent* TargetComp, FActiveStatusEffect& EffectData) const
{
	if (EffectData.StackCount >= 5)
	{
		TargetComp->SetFrozen(true);
	}

	if (!TargetComp->GetFrozen())
	{
		EffectData.RemainingDuration = Duration;	//Reinicia contador de effecto

		float TotalSlow = SlowPercentage;

		if (EffectData.StackCount > 0)
		{
			TotalSlow += EffectData.StackCount / 10;
		}

		TargetComp->SetSpeed(TargetComp->GetMaxSpeed() * TotalSlow - TargetComp->GetSpeed());
		TargetComp->SetHealth(TargetComp->GetHealth() - Damage);
	}
	else
	{
		TargetComp->SetArmor(0.7f);
		TargetComp->SetSpeed(0.0f);
		TargetComp->SetHealth(TargetComp->GetHealth() - Damage);
	}

	FVector SpawnLocation = TargetComp->GetOwner()->GetActorLocation() + FVector(0.0f, 0.0f, 90.0f);
	ShowDamageNumbers(SpawnLocation, Damage, ElementColor, TargetComp->GetWorld());
	return;
}

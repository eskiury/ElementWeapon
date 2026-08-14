#pragma once

#include "CoreMinimal.h"
#include "../ElementalAction_Muzzle.h"
#include "ElementalAction_Muzzle_Freeze.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENTWEAPON_API UElementalAction_Muzzle_Freeze : public UElementalAction_Muzzle
{
	GENERATED_BODY()

	float SlowPercentage = 0.1f;
	float TickInterval = 2.0f;
	float Damage = 1.0f;
	float Duration = 10.0f;

	virtual void ExecuteMuzzleModifier(AActor* TargetActor, const FHitResult& HitResult) const override;

public:

	virtual void OnStatusTick(class UStatusComponent* TargetComp, struct FActiveStatusEffect& EffectData) const override;

	virtual void OnStatusExpired(class UStatusComponent* TargetComp, const struct FActiveStatusEffect& EffectData) const override;

	virtual void OnStatusHitted(class UStatusComponent* TargetComp, struct FActiveStatusEffect& EffectData) const override;

};
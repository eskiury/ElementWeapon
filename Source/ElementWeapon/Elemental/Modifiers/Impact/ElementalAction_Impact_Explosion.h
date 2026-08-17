#pragma once

#include "CoreMinimal.h"
#include "../ElementalAction_Impact.h"
#include "ElementalAction_Impact_Explosion.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable, EditInlineNew)
class ELEMENTWEAPON_API UElementalAction_Impact_Explosion : public UElementalAction_Impact
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Explosion")
	float ExplosionRadius = 300.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Explosion")
	float RadialDamage = 10.0f;

	virtual TSet<AActor*> ExecuteImpactModifier(const FHitResult& HitResult, const UWorld* World, bool& bOutShouldDestroy) const override;

};

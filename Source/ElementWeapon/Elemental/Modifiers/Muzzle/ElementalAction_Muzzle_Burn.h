#pragma once

#include "CoreMinimal.h"
#include "../ElementalAction_Muzzle.h"
#include "ElementalAction_Muzzle_Burn.generated.h"


UCLASS(Abstract, Blueprintable, EditInlineNew)
class ELEMENTWEAPON_API UElementalAction_Muzzle_Burn : public UElementalAction_Muzzle
{
	GENERATED_BODY()

	float Damage = 5.0f;
	float TickInterval = 1.0f;
	float Duration = 3.0f;
	
	virtual void ExecuteMuzzleModifier(AActor* TargetActor, const FHitResult& HitResult) const override;

public:

	virtual void OnStatusTick(class UStatusComponent* TargetComp, struct FActiveStatusEffect& EffectData) const override;

	virtual void OnStatusExpired(class UStatusComponent* TargetComp, const struct FActiveStatusEffect& EffectData) const override;

	virtual void OnStatusHitted(class UStatusComponent* TargetComp, struct FActiveStatusEffect& EffectData) const override;
};

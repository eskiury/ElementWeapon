#pragma once

#include "CoreMinimal.h"
#include "../ElementalAction_Muzzle.h"
#include "ElementalAction_Muzzle_Burn.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENTWEAPON_API UElementalAction_Muzzle_Burn : public UElementalAction_Muzzle
{
	GENERATED_BODY()
	
	virtual void ExecuteMuzzleModifier(AActor* TargetActor, const FHitResult& HitResult) const override;
};

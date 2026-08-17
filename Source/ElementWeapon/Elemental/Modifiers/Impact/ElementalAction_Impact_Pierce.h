#pragma once

#include "CoreMinimal.h"
#include "../ElementalAction_Impact.h"
#include "ElementalAction_Impact_Pierce.generated.h"

/**
 * 
 */

class UStatusComponent;

UCLASS(Abstract, Blueprintable, EditInlineNew)
class ELEMENTWEAPON_API UElementalAction_Impact_Pierce : public UElementalAction_Impact
{
	GENERATED_BODY()
	
protected:


	virtual TSet<AActor*> ExecuteImpactModifier(const FHitResult& HitResult, const UWorld* World, bool& bOutShouldDestroy) const override;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Pierce")
	int32 MaxPierceCount = 3;
};

#pragma once

#include "CoreMinimal.h"
#include "../ElementalAction.h"
#include "ElementalAction_Impact.generated.h"


UCLASS(Abstract, EditInlineNew, Blueprintable)
class ELEMENTWEAPON_API UElementalAction_Impact : public UElementalAction
{
	GENERATED_BODY()

public:

	virtual TSet<AActor*> ExecuteImpactModifier(const FHitResult& HitResult, const UWorld* World, bool& bOutShouldDestroy) const { return TSet<AActor*>(); }
};

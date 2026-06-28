#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ElementalAction.generated.h"

/**
 * 
 */
UCLASS(EditInLineNew, DefaultToInstanced, Abstract)
class ELEMENTWEAPON_API UElementalAction : public UObject
{
	GENERATED_BODY()

	virtual void ExecuteAction(class UActorComponent* ComponentContext){}
	
};

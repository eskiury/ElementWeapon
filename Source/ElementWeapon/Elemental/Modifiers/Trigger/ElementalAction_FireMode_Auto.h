#pragma once

#include "CoreMinimal.h"
#include "../ElementalAction_FireMode.h"
#include "ElementalAction_FireMode_Auto.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENTWEAPON_API UElementalAction_FireMode_Auto : public UElementalAction_FireMode
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Fire Rate")
	float FireRate = 0.1f;

	virtual void ExecuteFireMode_Implementation(UWeaponTriggerComponent* TriggerComponent) const override;

};

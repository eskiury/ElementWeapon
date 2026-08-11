#pragma once

#include "CoreMinimal.h"
#include "../ElementalAction.h"
#include "ElementalAction_FireMode.generated.h"

class UWeaponTriggerComponent;

UCLASS(Abstract, EditInlineNew, Blueprintable)
class ELEMENTWEAPON_API UElementalAction_FireMode : public UElementalAction
{
	GENERATED_BODY()

public:
	//Para crear cosas en blueprints nada mas
	UFUNCTION(BlueprintNativeEvent, Category = "Fire Mode")
	void ExecuteFireMode(UWeaponTriggerComponent* TriggerComponent) const;

	virtual void ExecuteFireMode_Implementation(UWeaponTriggerComponent* TriggerComponent) const {}
};

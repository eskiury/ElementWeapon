#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ElementalAction_Trigger.generated.h"

/**
 * 
 */
//EditInLine es para poder instanciarlo directamente en el DataAsset
UCLASS(Abstract, EditInlineNew, DefaultToInstanced, Blueprintable)
class ELEMENTWEAPON_API UElementalAction_Trigger : public UObject
{
	GENERATED_BODY()

public:
	// Logica de tiempo o cadencia
	virtual void ExecuteTriggerModifier(class UWeaponTriggerComponent* TriggerComponent) const{}

	//Logica de accion de impacto, devuelve true si se tiene que destruir
	virtual bool ExecuteTriggerImpactModifier(const FHitResult HitResult, const UWorld* World) const { return true; }
};

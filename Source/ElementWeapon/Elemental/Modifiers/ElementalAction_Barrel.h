#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ElementalAction_Barrel.generated.h"

/**
 * 
 */
//EditInLine es para poder instanciarlo directamente en el DataAsset
UCLASS(Abstract, EditInlineNew, DefaultToInstanced, Blueprintable)
class ELEMENTWEAPON_API UElementalAction_Barrel : public UObject
{
	GENERATED_BODY()

public:
	// El contrato que ejecutará el Gatillo cuando el jugador dispare
	virtual void ExecuteBarrelModifier(class UWeaponBarrelComponent* BarrelComponent) const{}
};
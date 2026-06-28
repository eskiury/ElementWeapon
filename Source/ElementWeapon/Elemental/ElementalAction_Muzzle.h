#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ElementalAction_Muzzle.generated.h"

/**
 * 
 */
//EditInLine es para poder instanciarlo directamente en el DataAsset
UCLASS(Abstract, EditInlineNew, DefaultToInstanced, Blueprintable)
class ELEMENTWEAPON_API UElementalAction_Muzzle : public UObject
{
	GENERATED_BODY()

public:
	// El contrato que ejecutará el Gatillo cuando el jugador dispare
	virtual void ExecuteMuzzleModifier(class UWeaponMuzzleComponent* MuzzleComponent) const{}
};
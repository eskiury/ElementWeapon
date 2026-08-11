#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "ElementalAction.h"

//Preciso de este include para poder ejecutar la accion desde WeaponBarrelComponent
#include "Modifiers/ElementalAction_Barrel.h"
#include "Modifiers/ElementalAction_Muzzle.h"
#include "Modifiers/ElementalAction_FireMode.h"
#include "Modifiers/ElementalAction_Impact.h"

#include "ElementalDataAsset.generated.h"


UCLASS()
class ELEMENTWEAPON_API UElementalDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	// Reemplazas la antigua variable por estas dos:
	UPROPERTY(EditDefaultsOnly, Instanced, Category = "Trigger Modifiers")
	class UElementalAction_FireMode* FireModeAction;

	UPROPERTY(EditDefaultsOnly, Instanced, Category = "Trigger Modifiers")
	class UElementalAction_Impact* ImpactAction;

	UPROPERTY(EditAnywhere, Instanced, Category = "Cañon")
	class UElementalAction_Barrel* BarrelAction;

	UPROPERTY(EditAnywhere, Instanced, Category = "Punta")
	class UElementalAction_Muzzle* MuzzleAction;
};

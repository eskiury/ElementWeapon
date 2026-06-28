#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "ElementalAction.h"

#include "ElementalDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENTWEAPON_API UElementalDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Instanced, Category = "Gatillo")
	class UElementalAction_Trigger* TriggerAction;

	UPROPERTY(EditAnywhere, Instanced, Category = "Cañon")
	class UElementalAction_Barrel* BarrelAction;

	UPROPERTY(EditAnywhere, Instanced, Category = "Punta")
	class UElementalAction_Muzzle* MuzzleAction;
};

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ElementSelectionInterface.generated.h"

class UElementalDataAsset;
class AElementalPickup;

UINTERFACE(MinimalAPI, Blueprintable)
class UElementSelectionInterface : public UInterface
{
	GENERATED_BODY()
};

class ELEMENTWEAPON_API IElementSelectionInterface
{
	GENERATED_BODY()

public:
	// BlueprintImplementableEvent permite que el Blueprint cree el evento visual
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Weapon|Element")
	void StartElementSelection(UElementalDataAsset* ElementData, AElementalPickup* ElementPickup);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Weapon|Element")
	void CancelElementSelection();
};
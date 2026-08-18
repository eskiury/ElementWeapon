#pragma once

#include "CoreMinimal.h"
#include "../ElementalAction_Barrel.h"
#include "ElementalAction_Barrel_Arco.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable, EditInlineNew)
class ELEMENTWEAPON_API UElementalAction_Barrel_Arco : public UElementalAction_Barrel
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Arco")
	int32 ProjectileCount = 3;

	UPROPERTY(EditAnywhere, Category = "Arco")
	float SpreadAngle = 30.0f;

	virtual void ExecuteBarrelModifier(class UWeaponBarrelComponent* BarrelComponent) const override;

	FActorSpawnParameters SpawnParams;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> ProjectileClass;
};

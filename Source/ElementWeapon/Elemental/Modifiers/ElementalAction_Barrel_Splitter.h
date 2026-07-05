#pragma once

#include "CoreMinimal.h"
#include "../ElementalAction_Barrel.h"
#include "ElementalAction_Barrel_Splitter.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENTWEAPON_API UElementalAction_Barrel_Splitter : public UElementalAction_Barrel
{
	GENERATED_BODY()

	

protected:

	int32 ProjectileCount = 3;
	float SpreadAngle = 15.0f;

	virtual void ExecuteBarrelModifier(class UWeaponBarrelComponent* BarrelComponent) const override;

	FActorSpawnParameters SpawnParams;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> ProjectileClass;
	
};

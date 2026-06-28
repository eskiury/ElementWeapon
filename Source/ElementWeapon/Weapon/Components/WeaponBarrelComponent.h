#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../WeaponComponentInterface.h"
#include "WeaponBarrelComponent.generated.h"


UCLASS(Abstract, Blueprintable)
class ELEMENTWEAPON_API UWeaponBarrelComponent : public UActorComponent, public IWeaponComponentInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponBarrelComponent();

	virtual void InitializeComponentContext(class AWeaponBase* Weapon) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Weapon Context")
	class AWeaponBase* MyWeaponOwner;

	UPROPERTY(EditDefaultsOnly, Category = "Elemental Infusion")
	class UElementalDataAsset* InfusedElement;
	
};

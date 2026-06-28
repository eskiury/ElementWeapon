#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../WeaponComponentInterface.h"
#include "WeaponMuzzleComponent.generated.h"


UCLASS(Abstract, Blueprintable)
class ELEMENTWEAPON_API UWeaponMuzzleComponent : public UActorComponent, public IWeaponComponentInterface
{
	GENERATED_BODY()

public:
	UWeaponMuzzleComponent();

	virtual void InitializeComponentContext(class AWeaponBase* Weapon) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Weapon Context")
	class AWeaponBase* MyWeaponOwner;

	UPROPERTY(EditDefaultsOnly, Category = "Elemental Infusion")
	class UElementalDataAsset* InfusedElement;

};

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../WeaponComponentInterface.h"
#include "WeaponTriggerComponent.generated.h"


UCLASS( Abstract, Blueprintable )
class ELEMENTWEAPON_API UWeaponTriggerComponent : public UActorComponent, public IWeaponComponentInterface
{
	GENERATED_BODY()

public:	
	UWeaponTriggerComponent();

	virtual void InitializeComponentContext(class AWeaponBase* Weapon) override;

protected:
	//Guardamos la referencia de arma para usarla cuando este componente dispare
	UPROPERTY(BlueprintReadOnly, Category = "Weapon Context")
	class AWeaponBase* MyWeaponOwner;

	UPROPERTY(EditDefaultsOnly, Category = "Elemental Infusion")
	class UElementalDataAsset* InfusedElement;

};

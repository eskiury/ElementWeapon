#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Standard/WeaponComponentInterface.h"
#include "WeaponTriggerComponent.generated.h"


UCLASS( Abstract, Blueprintable )
class ELEMENTWEAPON_API UWeaponTriggerComponent : public UActorComponent, public IWeaponComponentInterface
{
	GENERATED_BODY()

	bool bHasFiredThisPull = false;

public:	
	UWeaponTriggerComponent();

	virtual void InitializeComponentContext(class AWeaponBase* Weapon) override;

	void PullTrigger();

	void ReleaseTrigger();

	void FireShot();
	FTimerHandle StreamTimerHandle;

	FORCEINLINE class UElementalDataAsset* GetInfusedElement() const { return InfusedElement; }
	void SetInfusedElement(UElementalDataAsset* NewElement) { InfusedElement = NewElement; }

protected:
	//Guardamos la referencia de arma para usarla cuando este componente dispare
	UPROPERTY(BlueprintReadOnly, Category = "Weapon Context")
	class AWeaponBase* MyWeaponOwner;

	UPROPERTY(EditDefaultsOnly, Category = "Elemental Infusion")
	class UElementalDataAsset* InfusedElement;

	UPROPERTY(EditAnywhere, Category = "Weapon Stats")
	float FireRate = 0.1f;

	

};

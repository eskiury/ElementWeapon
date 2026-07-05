#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../WeaponComponentInterface.h"
#include "WeaponBarrelComponent.generated.h"


UCLASS(Abstract, Blueprintable)
class ELEMENTWEAPON_API UWeaponBarrelComponent : public UActorComponent, public IWeaponComponentInterface
{
	GENERATED_BODY()

	FActorSpawnParameters SpawnParams;


public:	
	// Sets default values for this component's properties
	UWeaponBarrelComponent();

	virtual void InitializeComponentContext(class AWeaponBase* Weapon) override;

	void FireProjectile();

	void SpawnSingleActor(FVector Location, FRotator Rotation) const;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Weapon Context")
	class AWeaponBase* MyWeaponOwner;

	UPROPERTY(EditDefaultsOnly, Category = "Elemental Infusion")
	class UElementalDataAsset* InfusedElement;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> ProjectileClass;
	
};

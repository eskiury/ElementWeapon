#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Standard/WeaponComponentInterface.h"
#include "../Standard/WeaponProjectile.h"

#include "WeaponBarrelComponent.generated.h"

UENUM(BlueprintType)
enum class ShotingMode : uint8
{
	Projectile	UMETA(DisplayName = "Parabola"),
	Hitscan		UMETA(DisplayName = "Laser"),
	Stream		UMETA(DisplayName = "Lanzallamas")
};

UCLASS(Abstract, Blueprintable)
class ELEMENTWEAPON_API UWeaponBarrelComponent : public UActorComponent, public IWeaponComponentInterface
{
	GENERATED_BODY()

	void ExecuteProjectileShot(FVector Location, FRotator Rotation) const;
	void ExecuteHitsCanShot(FVector Location, FRotator Rotation) const;
	void ExecuteStreamShot(FVector Location, FRotator Rotation) const;

public:	
	// Sets default values for this component's properties
	UWeaponBarrelComponent();

	virtual void InitializeComponentContext(class AWeaponBase* Weapon) override;

	void FireBarrel();

	void DeliverShot(FVector Location, FRotator Rotation) const;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Weapon Context")
	class AWeaponBase* MyWeaponOwner;

	UPROPERTY(EditDefaultsOnly, Category = "Elemental Infusion")
	class UElementalDataAsset* InfusedElement;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWeaponProjectile> ProjectileClass;

	//Ojo que esto puede liarmela
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Shot Mode")
	ShotingMode ShootingMode;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Hitscan")TEnumAsByte<ECollisionChannel> HitscanChannel = ECC_Visibility;
	
};

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ElementalAction_Barrel.generated.h"


UENUM(BlueprintType)
enum class ShotingMode : uint8
{
	Projectile	UMETA(DisplayName = "Parabola"),
	Hitscan		UMETA(DisplayName = "Laser"),
	Stream		UMETA(DisplayName = "Lanzallamas")
};


//EditInLine es para poder instanciarlo directamente en el DataAsset
UCLASS(Abstract, EditInlineNew, DefaultToInstanced, Blueprintable)
class ELEMENTWEAPON_API UElementalAction_Barrel : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Shot Mode")
	ShotingMode ShotMode;

public:
	// El contrato que ejecutará el Gatillo cuando el jugador dispare
	virtual void ExecuteBarrelModifier(class UWeaponBarrelComponent* BarrelComponent) const{}

	ShotingMode GetShootingMode() { return ShotMode; }
};
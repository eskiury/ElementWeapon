#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponComponentInterface.h"

#include "WeaponBase.generated.h"

//UENUM(BlueprintType)
//enum class EWeapoPartSlot: uint8
//{
//	Trigger UMETA(DisplayName = "Gatillo"),
//	Barrel	UMETA(DisplayName = "Cañón"),
//	Muzzle	UMETA(DisplayName = "Punta")
//};



UENUM(BlueprintType)
enum class EWeaponSlot : uint8
{
	Trigger UMETA(DisplayName = "Gatillo"),
	Barrel	UMETA(DisplayName = "Cañón"),
	Muzzle	UMETA(DisplayName = "Punta")
};

UCLASS()
class ELEMENTWEAPON_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

	void EquipComponent(EWeaponSlot Slot, TSubclassOf<UActorComponent> NewComponentClass);

	UFUNCTION(BlueprintCallable, Category = "Weapon | Elements")
	void EquipElementToSlot(EWeaponSlot Slot, UElementalDataAsset* NewElement);

	UFUNCTION(BlueprintCallable, Category = "Weapon Actions")
	void ShotWeapon();

	UFUNCTION(BlueprintCallable, Category = "Weapon Actions")
	void StopShooting();


	//Getter del cañon
	FORCEINLINE class UWeaponBarrelComponent* GetCurrentBarrel() const { return CurrentBarrel; }

	//Getter de la punta
	FORCEINLINE class UWeaponMuzzleComponent* GetCurrentMuzzle() const { return CurrentMuzzle; }

	//Funciones parra modificar con cada componente del arma
	void ShotBarrel() const;
	void SetupPayload(class AWeaponProjectile* Projectile) const;
	void HandleHitscanImpact(const FHitResult& LineTrace) const;


private:
	void BeginPlay();

	UPROPERTY(Transient, VisibleAnywhere, Category = "Weapon Components")
	class UWeaponTriggerComponent* CurrentTrigger;

	UPROPERTY(Transient, VisibleAnywhere, Category = "Weapon Components")
	class UWeaponBarrelComponent* CurrentBarrel;

	UPROPERTY(Transient, VisibleAnywhere, Category = "Weapon Components")
	class UWeaponMuzzleComponent* CurrentMuzzle;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "WeaponSetup")
	TSubclassOf<class UWeaponTriggerComponent> DefaultTriggerClass;

	UPROPERTY(EditDefaultsOnly, Category = "WeaponSetup")
	TSubclassOf<class UWeaponBarrelComponent> DefaultBarrelClass;

	UPROPERTY(EditDefaultsOnly, Category = "WeaponSetup")
	TSubclassOf<class UWeaponMuzzleComponent> DefaultMuzzleClass;

};

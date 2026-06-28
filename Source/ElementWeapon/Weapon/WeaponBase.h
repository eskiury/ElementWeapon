// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "WeaponComponentInterface.h"
//#include"Components/WeaponTriggerComponent.h"
//#include"Components/WeaponBarrelComponent.h"
//#include"Components/WeaponMuzzleComponent.h"

#include "WeaponBase.generated.h"


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

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WeaponComponentInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWeaponComponentInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */

//class AWeaponBase;	//Forward declaration of WeaponBase class

class ELEMENTWEAPON_API IWeaponComponentInterface
{
	GENERATED_BODY()

public:

	virtual void InitializeComponentContext(class AWeaponBase* Weapon) = 0;	//Conlleva una forward declaration de la clase AWeaponBase, que es la clase base de las armas.
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
};

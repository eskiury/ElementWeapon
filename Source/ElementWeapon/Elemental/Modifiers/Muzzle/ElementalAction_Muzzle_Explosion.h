// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ElementalAction_Muzzle.h"
#include "ElementalAction_Muzzle_Explosion.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENTWEAPON_API UElementalAction_Muzzle_Explosion : public UElementalAction_Muzzle
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "Explosión")
	float ExplosionRadius = 300.0f;

	// La fuerza del empujón físico
	UPROPERTY(EditAnywhere, Category = "Explosión")
	float ExplosionForce = 50000.0f;

public:
	virtual bool ExecuteMuzzleModifier(class AWeaponProjectile* ProjectileContext, const FHitResult& HitResult) const override;
};

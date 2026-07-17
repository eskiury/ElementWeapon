// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ElementalAction_Trigger.h"
#include "ElementalAction_Trigger_Auto.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENTWEAPON_API UElementalAction_Trigger_Auto : public UElementalAction_Trigger
{
	GENERATED_BODY()

	
protected:

	UPROPERTY(EditAnywhere, Category = "Weapon Stats")
	float FireRate = 0.1f;

	virtual void ExecuteTriggerModifier(class UWeaponTriggerComponent* TriggerComponent) const override;

	FActorSpawnParameters SpawnParams;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> ProjectileClass;

};
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "../../Weapon/Standard/StatusComponent.h"
#include "ElementalAction_Muzzle.generated.h"

class ADamageNumberActor;

//EditInLine es para poder instanciarlo directamente en el DataAsset
UCLASS(Abstract, EditInlineNew, DefaultToInstanced, Blueprintable)
class ELEMENTWEAPON_API UElementalAction_Muzzle : public UObject
{
	GENERATED_BODY()

public:
	// El contrato que ejecutará el Gatillo cuando el jugador dispare
	virtual void ExecuteMuzzleModifier(AActor* TargetActor, const FHitResult& HitResult) const {}

	// En UElementalAction_Muzzle.h:

	// Se ejecuta en cada pulso de tiempo (DoT, congelar a los N ticks, contagiar rayo...)
	virtual void OnStatusTick(class UStatusComponent* TargetComp, struct FActiveStatusEffect& EffectData) const {}

	// Se ejecuta cuando el efecto llega a 0 segundos y se elimina (revertir velocidad, apagar fuego...)
	virtual void OnStatusExpired(class UStatusComponent* TargetComp, const struct FActiveStatusEffect& EffectData) const {}

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	TSubclassOf<class ADamageNumberActor> DamageActorClass;

	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	FLinearColor ElementColor = FLinearColor::White;
};
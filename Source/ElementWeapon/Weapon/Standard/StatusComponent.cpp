#include "StatusComponent.h"

#include "../../Elemental/Modifiers/ElementalAction_Muzzle.h"
// Sets default values for this component's properties
UStatusComponent::UStatusComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UStatusComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ActiveEffects.IsEmpty()) return;

	for (int i = ActiveEffects.Num() - 1; i >= 0; --i)
	{
		ActiveEffects[i].TimeSinceLastTick += DeltaTime;
		ActiveEffects[i].RemainingDuration -= DeltaTime;

		if (ActiveEffects[i].TickInterval > 0.0f && ActiveEffects[i].TimeSinceLastTick >= ActiveEffects[i].TickInterval)
		{
			ActiveEffects[i].TimeSinceLastTick -= ActiveEffects[i].TickInterval; //Reinicia el acumulador conservando el residuo
			ActiveEffects[i].TickCount++;//Incrementamos el contador de ticks 

			if (ActiveEffects[i].EffectClass)
			{
				const UElementalAction_Muzzle* ActionCDO = ActiveEffects[i].EffectClass->GetDefaultObject<UElementalAction_Muzzle>();
				if (ActionCDO)
				{
					ActionCDO->OnStatusTick(this, ActiveEffects[i]);
				}
			}
		}

		
		if (ActiveEffects[i].RemainingDuration <= 0.0f)
		{
			if (ActiveEffects[i].EffectClass)
			{
				const UElementalAction_Muzzle* ActionCDO = ActiveEffects[i].EffectClass->GetDefaultObject<UElementalAction_Muzzle>();
				if (ActionCDO)
				{
					ActionCDO->OnStatusExpired(this, ActiveEffects[i]);
				}
			}
			//Borramos posicion del array
			ActiveEffects.RemoveAt(i);
		}
	}
	

}

bool UStatusComponent::ApplyStatusEffect(TSubclassOf<UElementalAction_Muzzle> EffectClass, float Duration, float TickInterval)
{
	for (auto& Effect : ActiveEffects)
	{
		if (Effect.EffectClass == EffectClass)
		{
			//Effect.RemainingDuration = Duration;
			Effect.StackCount++;
			const UElementalAction_Muzzle* ActionCDO = Effect.EffectClass->GetDefaultObject<UElementalAction_Muzzle>();
			if (ActionCDO)
			{
				ActionCDO->OnStatusHitted(this, Effect);
			}
			return true;
		}
	}

	FActiveStatusEffect StatusEffect = { EffectClass, TickInterval, Duration, 0.0f };
	ActiveEffects.Add(StatusEffect);
	
	const UElementalAction_Muzzle* ActionCDO = ActiveEffects.Last().EffectClass->GetDefaultObject<UElementalAction_Muzzle>();

	if (ActionCDO)
	{
		ActionCDO->OnStatusHitted(this, ActiveEffects.Last());
	}

	return false;
}


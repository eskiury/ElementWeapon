#include "ElementalAction_Trigger_Auto.h"

#include "../../../Weapon/Components/WeaponTriggerComponent.h"

void UElementalAction_Trigger_Auto::ExecuteTriggerModifier(UWeaponTriggerComponent* TriggerComponent) const
{
    if (!TriggerComponent) return;

    UWorld* World = TriggerComponent->GetWorld();
    if (!World) return;

    if (!World->GetTimerManager().IsTimerActive(TriggerComponent->StreamTimerHandle))
    {
        World->GetTimerManager().SetTimer(
            TriggerComponent->StreamTimerHandle,
            TriggerComponent,
            &UWeaponTriggerComponent::FireShot,
            FireRate,
            true
        );
    }
}

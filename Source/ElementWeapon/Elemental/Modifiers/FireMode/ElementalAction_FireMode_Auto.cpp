#include "ElementalAction_FireMode_Auto.h"

#include "../../../Weapon/Components/WeaponTriggerComponent.h"


void UElementalAction_FireMode_Auto::ExecuteFireMode_Implementation(UWeaponTriggerComponent* TriggerComponent) const
{
    //Validate Trigger & World
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
    return;
}

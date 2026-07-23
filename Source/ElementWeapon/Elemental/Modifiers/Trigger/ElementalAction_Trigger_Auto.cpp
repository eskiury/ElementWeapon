#include "ElementalAction_Trigger_Auto.h"

#include "../../../Weapon/Components/WeaponTriggerComponent.h"

//Cosas de la explosion
#include "../../../Weapon/Standard/WeaponProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h" // Para poder pintar la esfera visual temporal

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

bool UElementalAction_Trigger_Auto::ExecuteTriggerImpactModifier(const FHitResult HitResult, const UWorld* World) const
{
	//if (ProjectileContext == nullptr || ProjectileContext->GetWorld() == nullptr) return true;
	//if (HitResult.) return true;

	if(World == nullptr)
	{
		return false;
	}

	// 1. Extraemos el punto exacto donde la bala chocó físicamente
	FVector ImpactPoint = HitResult.ImpactPoint;
	float ExplosionRadius = 300.0f;

	// 2. Pintamos una esfera roja transparente durante 2 segundos para "ver" la explosión
	DrawDebugSphere(
		World,
		ImpactPoint,
		ExplosionRadius,
		16, // Segmentos de la esfera
		FColor::Red,
		false, // No es persistente
		2.0f,  // Duración en segundos
		0,     // Prioridad de renderizado
		2.0f   // Grosor de la línea
	);

	// 3. Aplicamos física real en Unreal.
	// Esto buscará cualquier objeto con físicas activas dentro del radio y lo empujará con fuerza radial.
	UGameplayStatics::ApplyRadialDamage(
		World,
		10.0f, // Daño base (por si añades sistema de vida después)
		ImpactPoint,
		ExplosionRadius,
		UDamageType::StaticClass(),
		TArray<AActor*>(), // Ignorar actores (vacío)
		//ProjectileContext->GetOwner(), // Daño infligido por el Arma
		nullptr // Instigador
	);

	// OPCIONAL: Si tienes cajas con físicas activadas (Simulate Physics) en tu nivel,
	// puedes aplicar una fuerza de empuje directa aquí o dejar que el motor la reciba.

	// Devolvemos true para indicarle a la bala que proceda a destruirse
	return true;
}


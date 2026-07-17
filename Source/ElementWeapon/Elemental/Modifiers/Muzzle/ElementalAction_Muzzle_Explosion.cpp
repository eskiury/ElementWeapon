#include "ElementalAction_Muzzle_Explosion.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h" // Para poder pintar la esfera visual temporal
#include "GameFramework/Actor.h"

#include "../../../Weapon/WeaponProjectile.h"

bool UElementalAction_Muzzle_Explosion::ExecuteMuzzleModifier(AWeaponProjectile* ProjectileContext, const FHitResult& HitResult) const
{
	if (ProjectileContext == nullptr || ProjectileContext->GetWorld() == nullptr) return true;

	// 1. Extraemos el punto exacto donde la bala chocó físicamente
	FVector ImpactPoint = HitResult.ImpactPoint;

	// 2. Pintamos una esfera roja transparente durante 2 segundos para "ver" la explosión
	DrawDebugSphere(
		ProjectileContext->GetWorld(),
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
		ProjectileContext->GetWorld(),
		10.0f, // Daño base (por si añades sistema de vida después)
		ImpactPoint,
		ExplosionRadius,
		UDamageType::StaticClass(),
		TArray<AActor*>(), // Ignorar actores (vacío)
		ProjectileContext->GetOwner(), // Daño infligido por el Arma
		nullptr // Instigador
	);

	// OPCIONAL: Si tienes cajas con físicas activadas (Simulate Physics) en tu nivel,
	// puedes aplicar una fuerza de empuje directa aquí o dejar que el motor la reciba.

	// Devolvemos true para indicarle a la bala que proceda a destruirse
	return true;
}

#include "ElementalAction_Impact_Explosion.h"

#include "Engine/OverlapResult.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h" // Para poder pintar la esfera visual temporal


TSet<AActor*> UElementalAction_Impact_Explosion::ExecuteImpactModifier(const FHitResult& HitResult, const UWorld* World, bool& bOutShouldDestroy) const
{
	bOutShouldDestroy = true;
	TSet<AActor*> ProcessedActors;
	if (World == nullptr)
	{
		//Se devuelve vacio
		return ProcessedActors;
	}

	// 1. Extraemos el punto exacto donde la bala chocó físicamente
	FVector ImpactPoint = HitResult.ImpactPoint;

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
		RadialDamage, // Daño base (por si añades sistema de vida después)
		ImpactPoint,
		ExplosionRadius,
		UDamageType::StaticClass(),
		TArray<AActor*>(), // Ignorar actores (vacío)
		//ProjectileContext->GetOwner(), // Daño infligido por el Arma
		nullptr // Instigador
	);

	FCollisionShape ExplosionSphere = FCollisionShape::MakeSphere(ExplosionRadius);
	TArray<FOverlapResult>OverlapResults;
	FCollisionQueryParams Params;
	Params.bTraceComplex = true;

	bool bHasOverlaps = World->OverlapMultiByChannel(
		OverlapResults,
		ImpactPoint,
		FQuat::Identity,
		ECC_Pawn,	//O el canal de colision que usen tus enemigos
		ExplosionSphere,
		Params
	);

	if (bHasOverlaps)
	{
		for (const FOverlapResult& Overlap : OverlapResults)
		{
			AActor* HitActor = Overlap.GetActor();

			if (HitActor && !ProcessedActors.Contains(HitActor))
			{
				ProcessedActors.Add(HitActor);
			}
		}
	}

	return ProcessedActors;
}

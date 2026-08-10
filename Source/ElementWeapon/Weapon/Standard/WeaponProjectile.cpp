#include "WeaponProjectile.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "../../Elemental/Modifiers/ElementalAction_Muzzle.h"
#include "../../Elemental/Modifiers/ElementalAction_Trigger.h"
#include "StatusComponent.h"


AWeaponProjectile::AWeaponProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("Colision"));
	
	CollisionComp->bHiddenInGame = false;//Debug
	
	if (CollisionComp)
	{
		SetRootComponent(CollisionComp);
	}

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(CollisionComp);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	//Cosas para probar
	ProjectileMovement->InitialSpeed = 1000.0f;
}

void AWeaponProjectile::NotifyHit(
	UPrimitiveComponent* MyComp,
	AActor* Other,
	UPrimitiveComponent* OtherComp,
	bool bSelfMoved,
	FVector HitLocation,
	FVector HitNormal,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	// 1. Filtro de seguridad (no chocamos con nosotros mismos ni con el jugador)
	if (Other->GetOwner() == GetOwner() || Other == GetOwner()->GetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bala chocando consigo misma o con el jugador"));
		return;
	}

	TSet<AActor*> AffectedActors;

	// 2. Determinar qué actores han sido afectados según si hay Gatillo especial o no
	if (TriggerImpactAction != nullptr)
	{
		// Si es una explosión/barrido, el Gatillo nos devuelve todos los atrapados en el radio
		AffectedActors = TriggerImpactAction->ExecuteTriggerImpactModifier(Hit, GetWorld());
	}
	else
	{
		// Si es una bala estándar sin Gatillo, el único afectado es el actor con el que chocamos directamente
		if (Other != nullptr)
		{
			AffectedActors.Add(Other);
		}
	}

	// 3. Aplicar el efecto elemental (MuzzleAction) a todos los actores del conjunto
	if (MuzzleAction != nullptr && !AffectedActors.IsEmpty())
	{
		for (AActor* Actor : AffectedActors)
		{
			if (Actor && Actor->FindComponentByClass<UStatusComponent>())
			{
				MuzzleAction->ExecuteMuzzleModifier(Actor, Hit);
			}
		}
	}

	// 4. Una vez procesado el impacto y los efectos, la bala se destruye
	Destroy();
}



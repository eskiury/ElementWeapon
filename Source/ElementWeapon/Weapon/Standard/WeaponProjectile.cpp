#include "WeaponProjectile.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "../../Elemental/Modifiers/ElementalAction_Muzzle.h"
#include "../../Elemental/Modifiers/ElementalAction_Impact.h"
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
	// 1. Filtro de seguridad garantizado
	if (Other == nullptr) return;

	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		// Si chocamos con nosotros mismos, con nuestro dueño (el jugador) o con el dueño de nuestro dueño
		if (Other == MyOwner || Other->GetOwner() == MyOwner || (MyOwner->GetOwner() && Other == MyOwner->GetOwner()))
		{
			return;
		}
	}

	bool bShouldDestroy = true;
	TSet<AActor*> AffectedActors;

	// 2. Determinar qué actores han sido afectados según si hay Gatillo especial o no
	if (ImpactAction != nullptr)
	{
		AffectedActors = ImpactAction->ExecuteImpactModifier(Hit, GetWorld(), bShouldDestroy);
	}
	else
	{
		AffectedActors.Add(Other);
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

	// 4. Destrucción condicional
	if (bShouldDestroy)
	{
		Destroy();
	}
}



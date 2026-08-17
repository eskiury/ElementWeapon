#include "WeaponProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "StatusComponent.h"
#include "../../Elemental/Modifiers/ElementalAction_Muzzle.h"
#include "../../Elemental/Modifiers/ElementalAction_Impact.h"
#include "../../Elemental/Modifiers/Impact/ElementalAction_Impact_Pierce.h"

AWeaponProjectile::AWeaponProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("Colision"));
	CollisionComp->bHiddenInGame = false;
	SetRootComponent(CollisionComp);

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(CollisionComp);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 1000.0f;
}

// Se ejecuta DESPUÉS del spawn desde AWeaponBase
void AWeaponProjectile::InitializeProjectilePayload(UElementalAction_Muzzle* InMuzzle, UElementalAction_Impact* InImpact)
{
	MuzzleAction = InMuzzle;
	ImpactAction = InImpact;

	// Inicializamos el contador de perforaciones si la acción equipada es de tipo Pierce
	if (UElementalAction_Impact_Pierce* PierceMod = Cast<UElementalAction_Impact_Pierce>(ImpactAction))
	{
		RemainingPierces = PierceMod->MaxPierceCount;
	}
	else
	{
		RemainingPierces = 0;
	}
}

// 1. CUANDO TOCA A UN ENEMIGO (OVERLAP - NUNCA FRENA LA BALA)
void AWeaponProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor == nullptr) return;

	// Filtros de seguridad (ignorar jugador y dueño)
	AActor* MyOwner = GetOwner();
	if (MyOwner && (OtherActor == MyOwner || OtherActor->GetOwner() == MyOwner))
	{
		return;
	}

	// Comprobamos si tiene vida/estados (es un enemigo)
	UStatusComponent* StatusComp = OtherActor->FindComponentByClass<UStatusComponent>();
	if (!StatusComp) return;

	// Si ya lo hemos atravesado antes, ignoramos
	if (PiercedActors.Contains(OtherActor)) return;
	PiercedActors.Add(OtherActor);

	// Preparamos un HitResult sintético en la posición del enemigo para las funciones de impacto
	FHitResult FakeHit;
	FakeHit.ImpactPoint = OtherActor->GetActorLocation();
	FakeHit.HitObjectHandle = FActorInstanceHandle(OtherActor);

	bool bShouldDestroy = true;
	TSet<AActor*> AffectedActors;

	// Si tenemos acción de impacto (Pierce, Explosion...)
	if (ImpactAction != nullptr)
	{
		AffectedActors = ImpactAction->ExecuteImpactModifier(FakeHit, GetWorld(), bShouldDestroy);

		// Si es una acción de penetración
		if (ImpactAction->IsA<UElementalAction_Impact_Pierce>())
		{
			RemainingPierces--;
			// Solo destruimos la bala si agotó sus cargas
			if (RemainingPierces <= 0)
			{
				bShouldDestroy = true;
			}
			else
			{
				bShouldDestroy = false; // Continúa volando a través de él
			}
		}
	}
	else
	{
		// Si es una bala estándar sin mod de impacto, se destruye al primer enemigo
		AffectedActors.Add(OtherActor);
		bShouldDestroy = true;
	}

	// Aplicamos el efecto de la Punta (Fuego, Hielo...)
	if (MuzzleAction != nullptr && !AffectedActors.IsEmpty())
	{
		for (AActor* Actor : AffectedActors)
		{
			if (Actor && Actor->FindComponentByClass<UStatusComponent>())
			{
				MuzzleAction->ExecuteMuzzleModifier(Actor, FakeHit);
			}
		}
	}

	// Si agotó las cargas o no era perforante, se destruye
	if (bShouldDestroy)
	{
		Destroy();
	}
}

// 2. CUANDO CHOCA CONTRA UNA PARED O SUELO (BLOCK - DESTRUCCIÓN INMEDIATA)
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
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	if (Other == nullptr) return;

	// Si choca contra cualquier objeto sólido del mapa, se destruye de inmediato
	Destroy();
}
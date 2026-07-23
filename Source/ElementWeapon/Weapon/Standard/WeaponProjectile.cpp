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
	//UE_LOG(LogTemp, Warning, TEXT("PADRE BALA: %s"), *GetOwner()->GetName());

	//Comprobamos que no estamos chocando con nosotros mismos
	if (Other->GetOwner() == GetOwner() || Other == GetOwner()->GetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bala chocando con sigo misma o con jugador"));
		return;
	}
	if (MuzzleAction != nullptr)
	{
		//Comprobamos que sea un enemigo
		UStatusComponent* StatusEnemy = Other->FindComponentByClass<UStatusComponent>();
		if (StatusEnemy != nullptr)
		{
			MuzzleAction->ExecuteMuzzleModifier(Other, Hit);
		}
		//Se podria meter un else para interaccionar con el entorno
		//Flow else otro componente quemar pared...
	}
		
	//Empieza logica del trigger
	if (TriggerImpactAction == nullptr)
	{
		//UE_LOG(LogTemp, Log, TEXT("Bala destruida por impacto"));
		Destroy();
		return;
	}
	else
	{
		bool bShouldDestroy = TriggerImpactAction->ExecuteTriggerImpactModifier(Hit, GetOwner()->GetWorld());
		
		if (bShouldDestroy)
		{
			Destroy();
			return;
		}
	}
}



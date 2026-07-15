#include "WeaponProjectile.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "../Elemental/ElementalAction_Muzzle.h"


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
	ProjectileMovement->InitialSpeed = 500.0f;
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
	UE_LOG(LogTemp, Warning, TEXT("PADRE BALA: %s"), *GetOwner()->GetName());

	//Comprobamos que no estamos chocando con nosotros mismos
	if (Other->GetOwner() == GetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bala chocando con sigo misma"));
		return;
	}
		

	if (MuzzleAction == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Bala destruida por impacto"));
		Destroy();
		return;
	}
	else
	{
		bool bShouldDestroy = MuzzleAction->ExecuteMuzzleModifier(this, Hit);
		if (bShouldDestroy)
		{
			Destroy();
			return;
		}
	}
	//UE_LOG(LogTemp, Log, TEXT("Colision"));
	//return;
}



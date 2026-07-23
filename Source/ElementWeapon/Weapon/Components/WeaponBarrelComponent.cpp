#include "WeaponBarrelComponent.h"

#include "../../Elemental/ElementalDataAsset.h"
#include "../Standard/WeaponBase.h"
//#include "/Game/BP_FirstPersonCharacter.BP_FirstPersonCharacter"



UWeaponBarrelComponent::UWeaponBarrelComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	//Configuracion del Spawner de balas


}

void UWeaponBarrelComponent::InitializeComponentContext(AWeaponBase* Weapon)
{
	MyWeaponOwner = Weapon;
}

void UWeaponBarrelComponent::FireBarrel()
{
	//¿Tenemos elemento y acción geométrica en el cañón?
	if (InfusedElement != nullptr && InfusedElement->BarrelAction != nullptr)
	{
		//SÍ: Le cedemos el control al Splitter (él se encargará de llamar al spawn del proyectil
		InfusedElement->BarrelAction->ExecuteBarrelModifier(this);
	}
	else
	{
		//NO: Disparo limpio por defecto (Una sola bala recta)
		//Calculamos la posición y rotación actual del arma para el disparo estándar
		FVector SpawnLocation = GetOwner()->GetActorLocation();
		//FRotator SpawnRotation = GetOwner()->GetActorRotation();


		APawn* PlayerPawn = Cast<APawn>(MyWeaponOwner->GetOwner());
		FRotator SpawnRotation = FRotator::ZeroRotator;

		if (PlayerPawn && PlayerPawn->GetController())
		{
			// GetControlRotation() nos da exactamente el vector de mirada de la cámara del jugador
			SpawnRotation = PlayerPawn->GetController()->GetControlRotation();
		}
		else
		{
			// Si no hay jugador (ej: una torreta aliada), usamos la del arma por defecto
			SpawnRotation = GetOwner()->GetActorRotation();
		}

		//Llamamos a tu spawner físico para lanzar la bala única
		DeliverShot(SpawnLocation, SpawnRotation);
		//UE_LOG(LogTemp, Log, TEXT("Spawn Projectile	"));

	}
}
void UWeaponBarrelComponent::DeliverShot(FVector Location, FRotator Rotation) const
{
	//OJO con esto esta aqui porque sino al cambiar puede generar problemas de datos stale o congelados pero puede que haya que moverlo a donde se gestiona el cambio de elemento


	switch (ShootingMode)
	{
	case ShotingMode::Projectile:	

		ExecuteProjectileShot(Location, Rotation);
		break;

	case ShotingMode::Hitscan:

		ExecuteHitsCanShot(Location, Rotation);
		break;

	case ShotingMode::Stream:
		break;
	default:
		break;
	}
}

void UWeaponBarrelComponent::ExecuteProjectileShot(FVector Location, FRotator Rotation) const
{
	FActorSpawnParameters SpawnParams;

	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Owner = GetOwner();

	if (ProjectileClass == nullptr) return;
	//Spawneamos al actor (Projectile) en la posicion del arma sin colisionar unas con otras y con la rotacion alterada por el Modifier
	AWeaponProjectile* SpawnedActor = GetWorld()->SpawnActor<AWeaponProjectile>(ProjectileClass, Location, Rotation, SpawnParams);

	if (SpawnedActor)
	{
		// ¡Éxito! El actor se ha spawneado correctamente.
		MyWeaponOwner->SetupPayload(SpawnedActor);
	}
	else
	{
		// El spawn ha fallado
		UE_LOG(LogTemp, Warning, TEXT("Error: No se pudo spawnear el Actor."));
	}
}

void UWeaponBarrelComponent::ExecuteHitsCanShot(FVector Location, FRotator Rotation) const
{
	FCollisionQueryParams Params;

	FHitResult HitResult;
	Params.AddIgnoredActor(GetOwner());
	Params.AddIgnoredActor(GetOwner()->GetOwner());
	Params.bTraceComplex = true;
	Params.bReturnPhysicalMaterial = true;

	FVector  TraceEnd = Rotation.Vector() * 5000 + Location;

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Location,
		TraceEnd,
		HitscanChannel, // O usa tu ECC_Weapon
		Params
	);

	DrawDebugLine(GetWorld(), Location, bHit ? HitResult.ImpactPoint : TraceEnd,
		bHit ? FColor::Green : FColor::Red, false, 2.0f, 0, 1.0f);

	if (bHit)
	{
		MyWeaponOwner->HandleHitscanImpact(HitResult);
	}
}

void UWeaponBarrelComponent::ExecuteStreamShot(FVector Location, FRotator Rotation) const
{
	return;
}

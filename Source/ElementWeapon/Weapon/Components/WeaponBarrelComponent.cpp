#include "WeaponBarrelComponent.h"

#include "../../Elemental/ElementalDataAsset.h"
#include "../../Weapon/WeaponProjectile.h"
#include "../WeaponBase.h"
//#include "/Game/BP_FirstPersonCharacter.BP_FirstPersonCharacter"

UWeaponBarrelComponent::UWeaponBarrelComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	//Configuracion del Spawner de balas
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Owner = GetOwner();

}

void UWeaponBarrelComponent::InitializeComponentContext(AWeaponBase* Weapon)
{
	MyWeaponOwner = Weapon;
}

void UWeaponBarrelComponent::FireProjectile()
{
	//¿Tenemos elemento y acción geométrica en el cañón?
	if (InfusedElement != nullptr && InfusedElement->BarrelAction != nullptr)
	{
		//SÍ: Le cedemos el control al Splitter (él se encargará de llamar a SpawnSingleActor)
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
		SpawnSingleActor(SpawnLocation, SpawnRotation);
		//UE_LOG(LogTemp, Log, TEXT("Spawn Projectile	"));

	}
}
void UWeaponBarrelComponent::SpawnSingleActor(FVector Location, FRotator Rotation) const
{
	if (ProjectileClass == nullptr) return;
	//Spawneamos al actor (Projectile) en la posicion del arma sin colisionar unas con otras y con la rotacion alterada por el Modifier
	AWeaponProjectile* SpawnedActor = GetWorld()->SpawnActor<AWeaponProjectile>(ProjectileClass, Location, Rotation, SpawnParams);

	if (SpawnedActor)
	{
		// ¡Éxito! El actor se ha spawneado correctamente.
		MyWeaponOwner->ShotMuzzle(SpawnedActor);
	}
	else
	{
		// El spawn ha fallado
		UE_LOG(LogTemp, Warning, TEXT("Error: No se pudo spawnear el Actor."));
	}

}

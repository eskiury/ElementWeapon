#include "WeaponBarrelComponent.h"

#include "Kismet/GameplayStatics.h"


#include "../../Elemental/ElementalDataAsset.h"
#include "../Standard/WeaponBase.h"
#include "../../Elemental/Modifiers/Impact/ElementalAction_Impact_Pierce.h"
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
	// 1. Calculamos la posición real del Muzzle y la rotación convergente a la cruceta
	FVector SpawnLocation = FVector::ZeroVector;
	FRotator SpawnRotation = FRotator::ZeroRotator;

	if (!GetShootTransform(SpawnLocation, SpawnRotation))
	{
		return;
	}

	// 2. Si hay modificador de cañón, le pasamos la posición y rotación base calculadas
	if (InfusedElement != nullptr && InfusedElement->BarrelAction != nullptr)
	{
		InfusedElement->BarrelAction->ExecuteBarrelModifier(this, SpawnLocation, SpawnRotation);
	}
	else
	{
		// Disparo limpio sin modificador
		DeliverShot(SpawnLocation, SpawnRotation);
	}
}

void UWeaponBarrelComponent::DeliverShot(FVector Location, FRotator Rotation) const
{
	//InfusedElement->BarrelAction

	//if (!InfusedElement || !InfusedElement->BarrelAction) return;
	if (!InfusedElement || !InfusedElement->BarrelAction)
	{
		ExecuteProjectileShot(Location, Rotation);
		return;
	}

	switch (InfusedElement->BarrelAction->GetShootingMode())
	{
	case ShotingMode::Projectile:	

		ExecuteProjectileShot(Location, Rotation);
		break;

	case ShotingMode::Hitscan:

		ExecuteHitsCanShot(Location, Rotation);
		break;

	case ShotingMode::Stream:
		ExecuteStreamShot(Location,Rotation);
		break;
	default:
		ExecuteProjectileShot(Location, Rotation);
		break;
	}
}

bool UWeaponBarrelComponent::GetShootTransform(FVector& OutMuzzleLocation, FRotator& OutAimRotation) const
{
	UWorld* World = GetWorld();
	if (!World || !GetOwner()) return false;

	// 1. Origen por defecto (Fallback)
	OutMuzzleLocation = GetOwner()->GetActorLocation();
	bool bFoundSocket = false;

	// 2. Buscar el socket en SkeletalMesh o StaticMesh
	if (USkeletalMeshComponent* WeaponSkelMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>())
	{
		// Comprueba si tu socket se llama "Muzzle" o "MuzzleSocket" en el Editor
		if (WeaponSkelMesh->DoesSocketExist(TEXT("MuzzleSocket")))
		{
			OutMuzzleLocation = WeaponSkelMesh->GetSocketLocation(TEXT("MuzzleSocket"));
			bFoundSocket = true;
		}
		else if (WeaponSkelMesh->DoesSocketExist(TEXT("Muzzle")))
		{
			OutMuzzleLocation = WeaponSkelMesh->GetSocketLocation(TEXT("Muzzle"));
			bFoundSocket = true;
		}
	}
	else if (UStaticMeshComponent* WeaponStaticMesh = GetOwner()->FindComponentByClass<UStaticMeshComponent>())
	{
		if (WeaponStaticMesh->DoesSocketExist(TEXT("MuzzleSocket")))
		{
			OutMuzzleLocation = WeaponStaticMesh->GetSocketLocation(TEXT("MuzzleSocket"));
			bFoundSocket = true;
		}
		else if (WeaponStaticMesh->DoesSocketExist(TEXT("Muzzle")))
		{
			OutMuzzleLocation = WeaponStaticMesh->GetSocketLocation(TEXT("Muzzle"));
			bFoundSocket = true;
		}
	}

	if (!bFoundSocket)
	{
		UE_LOG(LogTemp, Warning, TEXT("[GetShootTransform] NO se encontró el socket. Usando posición base del Actor!"));
	}

	// 3. Obtener la cámara del jugador
	APlayerCameraManager* CamManager = UGameplayStatics::GetPlayerCameraManager(World, 0);
	if (!CamManager) return false;

	FVector CamLocation = CamManager->GetCameraLocation();
	FVector CamForward = CamManager->GetActorForwardVector();

	float MaxAimDistance = 10000.0f;
	FVector CamTraceEnd = CamLocation + (CamForward * MaxAimDistance);

	FCollisionQueryParams CamTraceParams;
	CamTraceParams.AddIgnoredActor(GetOwner());
	if (GetOwner()->GetOwner())
	{
		CamTraceParams.AddIgnoredActor(GetOwner()->GetOwner());
	}

	FHitResult CamHit;
	bool bHit = World->LineTraceSingleByChannel(
		CamHit,
		CamLocation,
		CamTraceEnd,
		ECC_Visibility,
		CamTraceParams
	);

	FVector TargetPoint = bHit ? CamHit.ImpactPoint : CamTraceEnd;

	// 4. Calcular dirección y rotación
	FVector ShootDirection = (TargetPoint - OutMuzzleLocation).GetSafeNormal();
	OutAimRotation = ShootDirection.Rotation();

	// -------------------------------------------------------------
	// 🔍 DEBUG VISUAL (Dura 2 segundos en pantalla al disparar)
	// -------------------------------------------------------------
	// 🟣 Esfera Magenta: Dónde cree el código que está la punta del cañón
	//DrawDebugSphere(World, OutMuzzleLocation, 6.0f, 8, FColor::Magenta, false, 2.0f, 0, 1.0f);

	// 🟡 Esfera Amarilla: Dónde apunta tu cruceta (Impacto de la cámara)
	//DrawDebugSphere(World, TargetPoint, 10.0f, 8, FColor::Yellow, false, 2.0f, 0, 1.0f);

	// 🟢 Línea Verde: Trayectoria real que unirá la boca del arma con el objetivo
	//DrawDebugLine(World, OutMuzzleLocation, TargetPoint, FColor::Green, false, 2.0f, 0, 1.5f);

	return true;
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
	UWorld* World = GetWorld();
	if (!World) return;

	// 1. Determinar el límite de perforaciones
	int32 MaxPierces = 1;
	if (InfusedElement && InfusedElement->ImpactAction)
	{
		if (UElementalAction_Impact_Pierce* PierceMod = Cast<UElementalAction_Impact_Pierce>(InfusedElement->ImpactAction))
		{
			MaxPierces = PierceMod->MaxPierceCount;
		}
	}

	// 2. Configurar parámetros de colisión
	FCollisionQueryParams Params;
	Params.bTraceComplex = true;
	Params.bReturnPhysicalMaterial = true;
	Params.AddIgnoredActor(GetOwner());
	if (GetOwner() && GetOwner()->GetOwner())
	{
		Params.AddIgnoredActor(GetOwner()->GetOwner());
	}

	FVector TraceDirection = Rotation.Vector();
	float MaxRange = 5000.0f;
	FVector CurrentStart = Location;
	FVector FinalDestination = Location + (TraceDirection * MaxRange);

	int32 PiercedCount = 0;
	bool bHitAnything = false;

	// 3. Bucle iterativo de perforación
	while (PiercedCount < MaxPierces)
	{
		FHitResult Hit;
		bool bHit = World->LineTraceSingleByChannel(
			Hit,
			CurrentStart,
			FinalDestination,
			HitscanChannel,
			Params
		);

		// Si no golpea nada en este segmento (disparo al aire)
		if (!bHit)
		{
			// 🔴 ROJO: Trayectoria hacia el aire libre
			DrawDebugLine(World, CurrentStart, FinalDestination, FColor::Red, false, 1.0f, 0, 1.0f);
			break;
		}

		bHitAnything = true;
		AActor* HitActor = Hit.GetActor();

		// Comprobar si es un enemigo (tiene StatusComponent)
		bool bIsEnemy = (HitActor && HitActor->FindComponentByClass<UStatusComponent>() != nullptr);

		if (bIsEnemy)
		{
			// 🟢 VERDE: Impacto y trayectoria hacia un enemigo atravesado
			DrawDebugLine(World, CurrentStart, Hit.ImpactPoint, FColor::Green, false, 1.0f, 0, 1.0f);
			DrawDebugSphere(World, Hit.ImpactPoint, 12.0f, 12, FColor::Green, false, 1.0f, 0, 1.0f);

			// Aplicar efectos elementales y de impacto
			MyWeaponOwner->HandleHitscanImpact(Hit);

			// Contabilizar la perforación e ignorar a este enemigo para el siguiente segmento
			PiercedCount++;
			Params.AddIgnoredActor(HitActor);

			// Avanzar el punto de inicio ligeramente por delante del punto de impacto para continuar la trayectoria
			CurrentStart = Hit.ImpactPoint + (TraceDirection * 2.0f);
		}
		else
		{
			// 🔵 AZUL: Impacto contra pared, suelo o estructura del escenario
			DrawDebugLine(World, CurrentStart, Hit.ImpactPoint, FColor::Blue, false, 1.0f, 0, 1.0f);
			DrawDebugSphere(World, Hit.ImpactPoint, 12.0f, 12, FColor::Blue, false, 1.0f, 0, 1.0f);

			// Se procesa el impacto en el entorno (por si hay explosión) y se corta el rayo
			MyWeaponOwner->HandleHitscanImpact(Hit);
			break;
		}
	}

	// Si el primer intento no golpeó absolutamente nada
	if (!bHitAnything && PiercedCount == 0)
	{
		// 🔴 ROJO: Disparo limpio al vacío
		DrawDebugLine(World, Location, FinalDestination, FColor::Red, false, 1.0f, 0, 1.0f);
	}
}

void UWeaponBarrelComponent::ExecuteStreamShot(FVector Location, FRotator Rotation) const
{
	UWorld* World = GetWorld();
	if (!World) return;

	// 1. Determinar el límite de objetivos que puede atravesar el chorro
	int32 MaxTargets = 3; // Límite base por defecto
	if (InfusedElement && InfusedElement->ImpactAction)
	{
		if (UElementalAction_Impact_Pierce* PierceMod = Cast<UElementalAction_Impact_Pierce>(InfusedElement->ImpactAction))
		{
			MaxTargets = PierceMod->MaxPierceCount;
		}
	}

	// 2. Configurar parámetros del barrido
	FCollisionQueryParams Params;
	Params.bTraceComplex = true;
	Params.bReturnPhysicalMaterial = true;
	Params.AddIgnoredActor(GetOwner());
	if (GetOwner() && GetOwner()->GetOwner())
	{
		Params.AddIgnoredActor(GetOwner()->GetOwner());
	}

	FVector TraceDir = Rotation.Vector();
	FVector CurrentStart = Location;
	FVector MaxDestination = Location + (TraceDir * StreamRange);
	FCollisionShape SphereShape = FCollisionShape::MakeSphere(StreamRadius);

	int32 TargetsAffected = 0;
	FVector FinalImpactEnd = MaxDestination;

	// 3. Barrido iterativo para traspasar enemigos hasta agotar las cargas
	while (TargetsAffected < MaxTargets)
	{
		FHitResult Hit;
		bool bHit = World->SweepSingleByChannel(
			Hit,
			CurrentStart,
			MaxDestination,
			FQuat::Identity,
			HitscanChannel,
			SphereShape,
			Params
		);

		if (!bHit)
		{
			// El chorro llegó al final de su rango sin chocar con nada más
			break;
		}

		AActor* HitActor = Hit.GetActor();
		bool bIsEnemy = (HitActor && HitActor->FindComponentByClass<UStatusComponent>() != nullptr);

		if (bIsEnemy)
		{
			// 🟢 VERDE: Esfera en el punto donde el cono toca al enemigo
			DrawDebugSphere(World, Hit.ImpactPoint, 15.0f, 12, FColor::Green, false, 1.5f, 0, 1.5f);

			MyWeaponOwner->HandleHitscanImpact(Hit);

			TargetsAffected++;
			Params.AddIgnoredActor(HitActor);

			// Avanzamos el inicio del barrido para continuar detectando detrás
			CurrentStart = Hit.Location + (TraceDir * 5.0f);
		}
		else
		{
			// 🔵 AZUL: Esfera en el obstáculo sólido que bloquea el chorro
			DrawDebugSphere(World, Hit.ImpactPoint, 15.0f, 12, FColor::Blue, false, 1.5f, 0, 1.5f);

			FinalImpactEnd = Hit.Location;
			MyWeaponOwner->HandleHitscanImpact(Hit);
			break;
		}
	}

	// 4. DIBUJAR LOS BORDES DEL VOLUMEN (Caja/Túnel 3D)
	FRotationMatrix RotMatrix(Rotation);
	FVector RightVector = RotMatrix.GetScaledAxis(EAxis::Y) * StreamRadius;
	FVector UpVector = RotMatrix.GetScaledAxis(EAxis::Z) * StreamRadius;

	// 4 esquinas del inicio del cañón
	FVector Start_TL = Location - RightVector + UpVector;
	FVector Start_TR = Location + RightVector + UpVector;
	FVector Start_BL = Location - RightVector - UpVector;
	FVector Start_BR = Location + RightVector - UpVector;

	// 4 esquinas del final del chorro (se detiene en paredes si hubo choque)
	FVector End_TL = FinalImpactEnd - RightVector + UpVector;
	FVector End_TR = FinalImpactEnd + RightVector + UpVector;
	FVector End_BL = FinalImpactEnd - RightVector - UpVector;
	FVector End_BR = FinalImpactEnd + RightVector - UpVector;

	// Cuadrado frontal (salida)
	DrawDebugLine(World, Start_TL, Start_TR, FColor::Orange, false, 1.0f, 0, 1.5f);
	DrawDebugLine(World, Start_TR, Start_BR, FColor::Orange, false, 1.0f, 0, 1.5f);
	DrawDebugLine(World, Start_BR, Start_BL, FColor::Orange, false, 1.0f, 0, 1.5f);
	DrawDebugLine(World, Start_BL, Start_TL, FColor::Orange, false, 1.0f, 0, 1.5f);

	// Cuadrado trasero (llegada)
	DrawDebugLine(World, End_TL, End_TR, FColor::Orange, false, 1.0f, 0, 1.5f);
	DrawDebugLine(World, End_TR, End_BR, FColor::Orange, false, 1.0f, 0, 1.5f);
	DrawDebugLine(World, End_BR, End_BL, FColor::Orange, false, 1.0f, 0, 1.5f);
	DrawDebugLine(World, End_BL, End_TL, FColor::Orange, false, 1.0f, 0, 1.5f);

	// Aristas longitudinales del túnel
	DrawDebugLine(World, Start_TL, End_TL, FColor::Orange, false, 1.0f, 0, 1.5f);
	DrawDebugLine(World, Start_TR, End_TR, FColor::Orange, false, 1.0f, 0, 1.5f);
	DrawDebugLine(World, Start_BL, End_BL, FColor::Orange, false, 1.0f, 0, 1.5f);
	DrawDebugLine(World, Start_BR, End_BR, FColor::Orange, false, 1.0f, 0, 1.5f);
}

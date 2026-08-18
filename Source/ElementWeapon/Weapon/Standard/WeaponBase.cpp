#include "WeaponBase.h"

#include"../Components/WeaponTriggerComponent.h"
#include"../Components/WeaponBarrelComponent.h"
#include"../Components/WeaponMuzzleComponent.h"

#include "WeaponProjectile.h"
#include "../../Elemental/ElementalDataAsset.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AWeaponBase::EquipComponent(EWeaponSlot Slot, TSubclassOf<UActorComponent> NewComponentClass)
{
	//Destrucción limpia del componente que ya existía en ese slot
	switch (Slot)
	{
	case EWeaponSlot::Trigger:
		if (CurrentTrigger != nullptr)
		{
			CurrentTrigger->UnregisterComponent();
			CurrentTrigger->DestroyComponent();
			CurrentTrigger = nullptr;
		}
		break;
	case EWeaponSlot::Barrel:
		if (CurrentBarrel != nullptr)
		{
			CurrentBarrel->UnregisterComponent();
			CurrentBarrel->DestroyComponent();
			CurrentBarrel = nullptr;
		}
		break;
	case EWeaponSlot::Muzzle:
		if (CurrentMuzzle != nullptr)
		{
			CurrentMuzzle->UnregisterComponent();
			CurrentMuzzle->DestroyComponent();
			CurrentMuzzle = nullptr;
		}
		break;
	}

	//Si la clase que nos pasan es nula, queríamos vaciar el slot. 
	//Como ya lo hemos limpiado arriba, salimos de la función inmediatamente.
	if (NewComponentClass == nullptr)
	{
		return;
	}

	//Instanciación genérica en memoria (Ocurre para cualquier slot)
	UActorComponent* NewComp = NewObject<UActorComponent>(this, NewComponentClass);
	if (NewComp == nullptr) return;

	//Asignación del nuevo componente a su puntero específico (Casting)
	switch (Slot)
	{
	case EWeaponSlot::Trigger:
		CurrentTrigger = Cast<UWeaponTriggerComponent>(NewComp);
		break;
	case EWeaponSlot::Barrel:
		CurrentBarrel = Cast<UWeaponBarrelComponent>(NewComp);
		break;
	case EWeaponSlot::Muzzle:
		CurrentMuzzle = Cast<UWeaponMuzzleComponent>(NewComp);
		break;
	}

	//Registro global del componente y inicialización del contrato (Interfaz)
	NewComp->RegisterComponent();

	IWeaponComponentInterface* InterfaceContext = Cast<IWeaponComponentInterface>(NewComp);
	if (InterfaceContext != nullptr)
	{
		InterfaceContext->InitializeComponentContext(this);
	}
}

void AWeaponBase::EquipElementToSlot(EWeaponSlot Slot, UElementalDataAsset* NewElement)
{
	switch (Slot)
	{
	case EWeaponSlot::Trigger:
		if (CurrentTrigger && NewElement != nullptr)
		{
			CurrentTrigger->SetInfusedElement(NewElement);
			UE_LOG(LogTemp, Log, TEXT("Elemento equipado en el GATILLO"));
		}
		break;
	case EWeaponSlot::Barrel:
		if (CurrentBarrel && NewElement != nullptr)
		{
			CurrentBarrel->SetInfusedElement(NewElement);
			UE_LOG(LogTemp, Log, TEXT("Elemento equipado en el GATILLO"));
		}
		break;
	case EWeaponSlot::Muzzle:
		if (CurrentMuzzle && NewElement != nullptr)
		{
			CurrentMuzzle->SetInfusedElement(NewElement);
			UE_LOG(LogTemp, Log, TEXT("Elemento equipado en el GATILLO"));
		}
		break;
	}
}

void AWeaponBase::ShotWeapon()
{
	if (CurrentTrigger != nullptr)
	{
		CurrentTrigger->PullTrigger();
	}
}

void AWeaponBase::StopShooting()
{
	if (CurrentTrigger != nullptr)
	{
		CurrentTrigger->ReleaseTrigger();
	}
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();	//Se necesita porque heredamos de la clase AActor que precisa su constructor base, o nos jodemos.

	//Inyectamos las clases por defecto
	EquipComponent(EWeaponSlot::Trigger, DefaultTriggerClass);
	EquipComponent(EWeaponSlot::Barrel, DefaultBarrelClass);
	EquipComponent(EWeaponSlot::Muzzle, DefaultMuzzleClass);


}

void AWeaponBase::ShotBarrel() const
{
	if (CurrentBarrel != nullptr)
	{
		CurrentBarrel->FireBarrel();
	}
}

void AWeaponBase::SetupPayload(AWeaponProjectile* Projectile) const
{
	if (Projectile == nullptr) return;

	UElementalAction_Muzzle* MuzzleMod = (CurrentMuzzle && CurrentMuzzle->GetInfusedElement())
		? CurrentMuzzle->GetInfusedElement()->MuzzleAction : nullptr;

	UElementalAction_Impact* ImpactMod = (CurrentTrigger && CurrentTrigger->GetInfusedElement())
		? CurrentTrigger->GetInfusedElement()->ImpactAction : nullptr;

	Projectile->InitializeProjectilePayload(MuzzleMod, ImpactMod);
}

void AWeaponBase::HandleHitscanImpact(const FHitResult& LineTrace) const
{
	TSet<AActor*> AffectedActors;
	bool bShouldDestroy = false;

	// 1. Extraemos la Acción del Gatillo (si existe)
	UElementalAction_Impact* TriggerAction = nullptr;
	if (CurrentTrigger && CurrentTrigger->GetInfusedElement())
	{
		TriggerAction = CurrentTrigger->GetInfusedElement()->ImpactAction;
	}

	// 2. Determinar qué actores han sido afectados
	if (TriggerAction != nullptr)
	{
		// Si hay un Gatillo especial (ej: explosión), él calcula los actores atrapados en el radio del impacto
		AffectedActors = TriggerAction->ExecuteImpactModifier(LineTrace, GetWorld(), bShouldDestroy);
	}
	else
	{
		// Si es un disparo directo sin área, el único afectado es el actor golpeado directamente por el rayo
		AActor* ActorHit = LineTrace.GetActor();
		if (ActorHit != nullptr)
		{
			AffectedActors.Add(ActorHit);
		}
	}

	// 3. Extraemos la Acción de la Punta (Muzzle)
	UElementalAction_Muzzle* MuzzleAction = nullptr;
	if (CurrentMuzzle && CurrentMuzzle->GetInfusedElement())
	{
		MuzzleAction = CurrentMuzzle->GetInfusedElement()->MuzzleAction;
	}

	// 4. Aplicamos el efecto elemental a todos los actores del conjunto que tengan StatusComponent
	if (MuzzleAction != nullptr && !AffectedActors.IsEmpty())
	{
		for (AActor* Actor : AffectedActors)
		{
			if (Actor && Actor->FindComponentByClass<UStatusComponent>())
			{
				MuzzleAction->ExecuteMuzzleModifier(Actor, LineTrace);
			}
		}
	}
}



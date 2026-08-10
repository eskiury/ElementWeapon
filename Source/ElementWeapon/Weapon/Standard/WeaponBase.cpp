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
	if (Projectile == nullptr || CurrentMuzzle == nullptr || CurrentTrigger == nullptr) return;

	UElementalDataAsset* MuzzleElement = CurrentMuzzle->GetInfusedElement();

	if (MuzzleElement != nullptr)
	{
		// Inyectamos la acción lógica de efecto sobre enemigo (Ej: Crear quemar o envenenar)
		Projectile->MuzzleAction = MuzzleElement->MuzzleAction;

		// Inyectamos el Data Asset completo para efectos visuales o tipo de daño
		Projectile->ElementalData = MuzzleElement; //OOOOOOOOJOOOOOOOOO Puede que esto tenga que ser el del trigger hay que ver a futuro
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Disparo Neutro: La Punta actual no tiene ningun elemento infusionado."));
	}

	UElementalDataAsset* TriggerElement = CurrentTrigger->GetInfusedElement();

	if (TriggerElement != nullptr)
	{
		//Inyectamos la acion lógica del impacto (Ej: Explotar o rebotar)
		Projectile->TriggerImpactAction = TriggerElement->TriggerAction;

		// Inyectamos el Data Asset completo para efectos visuales o tipo de daño
		//Projectile->ElementalData = TriggerElement; //OOOOOOOOJOOOOOOOOO Puede que esto tenga que ser el del trigger hay que ver a futuro
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Disparo Neutro: El gatillo actual no tiene ningun elemento infusionado."));
	}
}

void AWeaponBase::HandleHitscanImpact(const FHitResult& LineTrace) const
{
	TSet<AActor*> AffectedActors;

	// 1. Extraemos la Acción del Gatillo (si existe)
	UElementalAction_Trigger* TriggerAction = nullptr;
	if (CurrentTrigger && CurrentTrigger->GetInfusedElement())
	{
		TriggerAction = CurrentTrigger->GetInfusedElement()->TriggerAction;
	}

	// 2. Determinar qué actores han sido afectados
	if (TriggerAction != nullptr)
	{
		// Si hay un Gatillo especial (ej: explosión), él calcula los actores atrapados en el radio del impacto
		AffectedActors = TriggerAction->ExecuteTriggerImpactModifier(LineTrace, GetWorld());
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



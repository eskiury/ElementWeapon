#include "WeaponBase.h"

#include"Components/WeaponTriggerComponent.h"
#include"Components/WeaponBarrelComponent.h"
#include"Components/WeaponMuzzleComponent.h"

#include "WeaponProjectile.h"
#include "../Elemental/ElementalDataAsset.h"

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

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();	//Se necesita porque heredamos de la clase AActor que precisa su constructor base, o nos jodemos.

	//Inyectamos las clases por defecto
	EquipComponent(EWeaponSlot::Trigger, DefaultTriggerClass);
	EquipComponent(EWeaponSlot::Barrel, DefaultBarrelClass);
	EquipComponent(EWeaponSlot::Muzzle, DefaultMuzzleClass);

	if (CurrentTrigger != nullptr)
	{
		CurrentTrigger->PullTrigger();

	}
}

void AWeaponBase::ShotBarrel() const
{
	if (CurrentBarrel != nullptr)
	{
		CurrentBarrel->FireProjectile();
	}
}

void AWeaponBase::ShotMuzzle(AWeaponProjectile* Projectile) const
{
	if (Projectile == nullptr || CurrentMuzzle == nullptr) return;

	UElementalDataAsset* MuzzleElement = CurrentMuzzle->GetInfusedElement();

	if (MuzzleElement != nullptr)
	{
		// Inyectamos la acción lógica de impacto (Ej: Crear charco de veneno)
		Projectile->MuzzleAction = MuzzleElement->MuzzleAction;

		// Inyectamos el Data Asset completo para efectos visuales o tipo de daño
		Projectile->ElementalData = MuzzleElement;


		// ─── LOG DE PRUEBA TEMPORAL ───
		// %s espera un texto. Usar *Objeto->GetName() nos da el nombre del archivo en Unreal
		//UE_LOG(LogTemp, Log, TEXT("¡PROYECTIL CARGADO! Se ha inyectado el elemento: %s"), *MuzzleElement->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Disparo Neutro: La Punta actual no tiene ningun elemento infusionado."));
	}

}



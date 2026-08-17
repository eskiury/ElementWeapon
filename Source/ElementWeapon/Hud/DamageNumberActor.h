#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageNumberActor.generated.h"

class UWidgetComponent;
class UProjectileMovementComponent;

UCLASS()
class ELEMENTWEAPON_API ADamageNumberActor : public AActor
{
	GENERATED_BODY()

public:
	ADamageNumberActor();

	// Función de entrada que recibe el daño y el color elemental
	void SetDamageNumber(float DamageAmount, FLinearColor DamageColor);

protected:
	virtual void BeginPlay() override;

	// Componente visual que dibuja el widget UMG en el mundo 3D
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWidgetComponent* DamageWidgetComp;

	// Componente físico que calcula la parábola y gravedad
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* MovementComp;

	// --- FÍSICAS DE DISPERSIÓN ---
	UPROPERTY(EditDefaultsOnly, Category = "Damage Physics")
	float UpwardVelocity = 380.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage Physics")
	float HorizontalSpread = 180.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage Physics")
	float CustomGravityScale = 1.2f;

	// --- ESCALADO POR DAÑO ---
	UPROPERTY(EditDefaultsOnly, Category = "Damage Scaling")
	float MinDamage = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage Scaling")
	float MaxDamage = 500.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage Scaling")
	float MinDamageScale = 0.75f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage Scaling")
	float MaxDamageScale = 1.8f;

	// --- ESCALADO POR DISTANCIA ---
	// Distancia ideal en cm (10 metros) donde el tamaño es 1:1
	UPROPERTY(EditDefaultsOnly, Category = "Distance Scaling")
	float ReferenceDistance = 1000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Distance Scaling")
	float MinDistanceScale = 0.35f;

	UPROPERTY(EditDefaultsOnly, Category = "Distance Scaling")
	float MaxDistanceScale = 1.4f;
};
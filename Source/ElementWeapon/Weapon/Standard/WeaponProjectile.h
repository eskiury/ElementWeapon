#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponProjectile.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
class UElementalAction_Muzzle;
class UElementalAction_Impact;

UCLASS()
class ELEMENTWEAPON_API AWeaponProjectile : public AActor
{
	GENERATED_BODY()

public:
	AWeaponProjectile();

	// Función para inyectarle los modificadores al nacer
	void InitializeProjectilePayload(UElementalAction_Muzzle* InMuzzle, UElementalAction_Impact* InImpact);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovement;

	// Modificadores elementales
	UPROPERTY()
	UElementalAction_Muzzle* MuzzleAction;

	UPROPERTY()
	UElementalAction_Impact* ImpactAction;

	// Estado interno de penetración (UNA SOLA VEZ)
	int32 RemainingPierces = 0;

	UPROPERTY()
	TSet<AActor*> PiercedActors;

	// Para cuando atraviesa enemigos (Overlap)
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	// Para cuando choca contra paredes o suelos (Block)
	virtual void NotifyHit(
		UPrimitiveComponent* MyComp,
		AActor* Other,
		UPrimitiveComponent* OtherComp,
		bool bSelfMoved,
		FVector HitLocation,
		FVector HitNormal,
		FVector NormalImpulse,
		const FHitResult& Hit) override;
};
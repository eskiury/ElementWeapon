#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponProjectile.generated.h"

UCLASS()
class ELEMENTWEAPON_API AWeaponProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// CARGA ÚTIL (Pública para que el Cañón pueda inyectar los datos al spawnear)
	UPROPERTY(BlueprintReadWrite, Category = "Projectile | Payload")
	class UElementalAction_Muzzle* MuzzleAction;

	UPROPERTY(BlueprintReadWrite, Category = "Projectile | Payload")
	class UElementalDataAsset* ElementalData;

	// Sets default values for this actor's properties
	AWeaponProjectile();

protected:
	// Componentes físicos (Visibles en el editor, protegidos en código)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile | Components")
	class USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile | Components")
	class UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile | Components")
	class UProjectileMovementComponent* ProjectileMovement;

//protected:
//	// Called when the game starts or when spawned
//	virtual void BeginPlay() override;
//
//public:	
//	// Called every frame
//	virtual void Tick(float DeltaTime) override;

};

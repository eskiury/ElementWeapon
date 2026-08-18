#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ElementalPickup.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class URotatingMovementComponent;
class UElementalDataAsset;
UCLASS()
class ELEMENTWEAPON_API AElementalPickup : public AActor
{
	GENERATED_BODY()

	bool CharacterOverlapping = false;
public:	
	// Sets default values for this actor's properties
	AElementalPickup();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	URotatingMovementComponent* RotatorComponent;

	UPROPERTY(EditInstanceOnly, Category = "Element")
	UElementalDataAsset* ElementData;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
};

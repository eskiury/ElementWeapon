#include "ElementalPickup.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "GameFramework/Character.h"
#include "../Weapon/Interfaces/ElementSelectionInterface.h"

// Sets default values
AElementalPickup::AElementalPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SetRootComponent(CollisionComp);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ElementMesh"));
	StaticMesh->SetupAttachment(CollisionComp);
	StaticMesh->SetCollisionProfileName(TEXT("NoCollision"));

	RotatorComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatorComp"));
	RotatorComponent->RotationRate = FRotator(0.0f, 90.0f, 0.0f); // Gira en Yaw
}

// Called when the game starts or when spawned
void AElementalPickup::BeginPlay()
{
	Super::BeginPlay();
	
}

void AElementalPickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (CharacterOverlapping || OtherActor == nullptr) return;

	if (OtherActor->Implements<UElementSelectionInterface>())
	{
		IElementSelectionInterface::Execute_StartElementSelection(OtherActor, ElementData, this);
		CharacterOverlapping = true;
	}
}

void AElementalPickup::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	if (!CharacterOverlapping || OtherActor == nullptr) return;
	if (OtherActor->Implements<UElementSelectionInterface>())
	{
		IElementSelectionInterface::Execute_CancelElementSelection(OtherActor);
		CharacterOverlapping = false;
	}
}

// Called every frame
void AElementalPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


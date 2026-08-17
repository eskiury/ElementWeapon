#include "DamageNumberActor.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"
#include "Blueprint/UserWidget.h"
#include "Misc/OutputDeviceNull.h"

ADamageNumberActor::ADamageNumberActor()
{
	// Desactivamos Tick para optimizar rendimiento (la física la maneja MovementComp)
	PrimaryActorTick.bCanEverTick = false;

	// 1. Inicializar WidgetComponent
	DamageWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("DamageWidgetComp"));
	SetRootComponent(DamageWidgetComp);
	DamageWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	DamageWidgetComp->SetDrawSize(FVector2D(150.0f, 50.0f));

	// 2. Inicializar ProjectileMovementComponent
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	MovementComp->UpdatedComponent = DamageWidgetComp;
	MovementComp->InitialSpeed = 0.0f;
	MovementComp->MaxSpeed = 2000.0f;
	MovementComp->bRotationFollowsVelocity = false;
	MovementComp->bShouldBounce = false;
	MovementComp->ProjectileGravityScale = CustomGravityScale;

	// 3. Tiempo de vida antes de autodestruirse en RAM
	InitialLifeSpan = 0.85f;
}

void ADamageNumberActor::BeginPlay()
{
	Super::BeginPlay();

	// 1. Obtener la orientación de la cámara del jugador
	APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	FVector ScreenRight = CameraManager ? CameraManager->GetActorRightVector() : FVector::RightVector;

	// 2. Calcular dispersión lateral aleatoria (-1 izquierda, +1 derecha)
	float RandomHorizontal = FMath::RandRange(-1.0f, 1.0f);

	// Filtro de zona muerta: evita que el texto suba en línea recta vertical pura
	if (FMath::Abs(RandomHorizontal) < 0.15f)
	{
		RandomHorizontal = (RandomHorizontal < 0.0f) ? -0.45f : 0.45f;
	}

	// 3. Aplicar impulso de lanzamiento
	float FinalUpward = UpwardVelocity + FMath::RandRange(-30.0f, 30.0f);
	FVector LaunchVelocity = (ScreenRight * (RandomHorizontal * HorizontalSpread)) + (FVector::UpVector * FinalUpward);

	MovementComp->Velocity = LaunchVelocity;
}

void ADamageNumberActor::SetDamageNumber(float DamageAmount, FLinearColor DamageColor)
{
	if (!DamageWidgetComp) return;

	UUserWidget* UserWidget = DamageWidgetComp->GetUserWidgetObject();
	if (!UserWidget) return;

	// 1. CÁLCULO DE ESCALA POR DAÑO (Interpolación lineal acotada)
	float DamageScale = FMath::GetMappedRangeValueClamped(
		FVector2D(MinDamage, MaxDamage),
		FVector2D(MinDamageScale, MaxDamageScale),
		DamageAmount
	);

	// 2. CÁLCULO DE ESCALA POR DISTANCIA (Perspectiva simulada)
	APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	float DistanceScale = 1.0f;

	if (CameraManager)
	{
		float DistanceToCamera = FVector::Dist(GetActorLocation(), CameraManager->GetCameraLocation());

		// Evitamos división por cero con un mínimo de seguridad (1 metro)
		DistanceToCamera = FMath::Max(DistanceToCamera, 100.0f);

		// Cuanto más lejos está el actor, menor es el factor resultante
		DistanceScale = ReferenceDistance / DistanceToCamera;
		DistanceScale = FMath::Clamp(DistanceScale, MinDistanceScale, MaxDistanceScale);
	}

	// 3. ESCALA COMBINADA TOTAL
	float FinalScale = DamageScale * DistanceScale;

	// 4. ENVÍO DE DATOS A UMG
	FOutputDeviceNull ar;
	FString Command = FString::Printf(
		TEXT("SetDamageValue %f (R=%f,G=%f,B=%f,A=%f) %f"),
		DamageAmount,
		DamageColor.R, DamageColor.G, DamageColor.B, DamageColor.A,
		FinalScale
	);

	UserWidget->CallFunctionByNameWithArguments(*Command, ar, nullptr, true);
}
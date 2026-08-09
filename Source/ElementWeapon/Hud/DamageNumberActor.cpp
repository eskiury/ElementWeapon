#include "DamageNumberActor.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "Misc/OutputDeviceNull.h"

ADamageNumberActor::ADamageNumberActor()
{
	PrimaryActorTick.bCanEverTick = true;

	//Creamos el componentes de Widget
	DamageWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("DamageWidgetComp"));
	SetRootComponent(DamageWidgetComp);

	//Renderizar elemento mirando a camara
	DamageWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	DamageWidgetComp->SetDrawSize(FVector2D(150.0f, 50.0f));

	//Autodestruccion
	InitialLifeSpan = 1.0f;
}

void ADamageNumberActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADamageNumberActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Elevar texto progresivamente
	FVector CurrentLocation = GetActorLocation();
	CurrentLocation.Z += 40 * DeltaTime;
	SetActorLocation(CurrentLocation);
}

void ADamageNumberActor::SetDamageNumber(float DamageAmount, FLinearColor DamageColor)
{
	if (DamageWidgetComp)
	{
		//Obtenemos la instancia real del Widget que está cargada dentro del componente
		UUserWidget* UserWidget = DamageWidgetComp->GetUserWidgetObject();
		if (UserWidget)
		{
			//Llamamos a la funcion del bleuprint que creamos antes
			FOutputDeviceNull ar;
			FString Command = FString::Printf(
				TEXT("SetDamageValue %f (R=%f,G=%f,B=%f,A=%f)"),
				DamageAmount,
				DamageColor.R, DamageColor.G, DamageColor.B, DamageColor.A
			);
			UserWidget->CallFunctionByNameWithArguments(*Command, ar, nullptr, true);
		}
	}
	return;
}


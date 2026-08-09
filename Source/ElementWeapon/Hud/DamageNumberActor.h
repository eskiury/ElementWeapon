#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageNumberActor.generated.h"

UCLASS()
class ELEMENTWEAPON_API ADamageNumberActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ADamageNumberActor();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UWidgetComponent* DamageWidgetComp;

public:	
	virtual void Tick(float DeltaTime) override;

	//Funcion para incializar numero desde fuera
	UFUNCTION(BlueprintCallable, Category = "Damage")

	void SetDamageNumber(float DamageAmount, FLinearColor DamageColor);

};

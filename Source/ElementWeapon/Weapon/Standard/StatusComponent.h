#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "StatusComponent.generated.h"

//Forward Declaration
class UElementalAction_Muzzle;

USTRUCT(BlueprintType)
struct FActiveStatusEffect
{
	GENERATED_USTRUCT_BODY()

	TSubclassOf<UElementalAction_Muzzle> EffectClass;
	float TickInterval;
	float RemainingDuration;

	float TimeSinceLastTick;
	int32 StackCount;
	int32 TickCount;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ELEMENTWEAPON_API UStatusComponent : public UActorComponent
{
	GENERATED_BODY()

	float Speed = 1.0f;
	float Health = 100.0f;
	float Size = 1.0f;
	float Damage = 1.0f;
	float Armor = 1.0f;

	float MaxSpeed = 300.0f;
	bool Frozen = false;

	UPROPERTY(VisibleAnywhere, Category = "Status Effects")
	TArray<FActiveStatusEffect> ActiveEffects;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	// Sets default values for this component's properties
	UStatusComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool ApplyStatusEffect(TSubclassOf<UElementalAction_Muzzle> EffectClass, float Duration, float TickInterval);

	//SETTERS & GETTERS
	FORCEINLINE float GetSpeed() const { return Speed; }
	FORCEINLINE void SetSpeed(float NewSpeed) 
	{
		if (NewSpeed <= 0) { Speed = 0; }
		else { Speed = NewSpeed; }
	}
	FORCEINLINE float GetMaxSpeed() const { return MaxSpeed; }

	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE void SetHealth(float NewHealth) 
	{
		if (NewHealth <= 0) { Health = 0; }
		else { Health = NewHealth; }
	}

	FORCEINLINE float GetSize() const { return Size; }
	FORCEINLINE void SetSize(float NewSize)
	{
		if (NewSize <= 0) { Size = 0; }
		else { Size = NewSize; }
	}

	FORCEINLINE float GetDamage() const { return Damage; }
	FORCEINLINE void SetDamage(float NewDamage)
	{
		if (NewDamage <= 0) { Damage = 0; }
		else { Damage = NewDamage / Armor; }
	}

	FORCEINLINE float GetArmor() const { return Armor; }
	FORCEINLINE void SetArmor(float NewArmor)
	{
		if (NewArmor <= 0) { Armor = 0; }
		else { Armor = NewArmor; }
	}

	FORCEINLINE bool GetFrozen() const { return Frozen; }
	FORCEINLINE void SetFrozen(bool NewFrozen) { Frozen = NewFrozen; }

};

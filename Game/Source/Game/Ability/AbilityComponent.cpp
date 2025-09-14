#include "Game/Ability/AbilityComponent.h"
#include "Game/Ability/AbilitySettings.h"
#include "Kismet/GameplayStatics.h"

UAbilityComponent::UAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
	if (const UAbilitySettings* Settings = GetDefault<UAbilitySettings>())
	{
		for (const TPair<FName, float>& Pair : Settings->DefaultAttributes)
		{
			Attributes.FindOrAdd(Pair.Key) = Pair.Value;
		}
	}

	if (AActor* Owner = GetOwner())
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &UAbilityComponent::OnAnyDamageTaken);
	}
}

float UAbilityComponent::GetAttribute(FName Name, float DefaultIfMissing) const
{
	if (const float* Found = Attributes.Find(Name))
	{
		return *Found;
	}
	return DefaultIfMissing;
}

void UAbilityComponent::SetAttribute(FName Name, float Value)
{
	Attributes.FindOrAdd(Name) = Value;
}

void UAbilityComponent::AddAttribute(FName Name, float Delta)
{
	Attributes.FindOrAdd(Name) += Delta;
}

void UAbilityComponent::ApplyDamageAmount(float Amount)
{
	const float OldHP = GetAttribute(TEXT("Health"));
	SetAttribute(TEXT("Health"), FMath::Max(0.f, OldHP - Amount));
}

void UAbilityComponent::OnAnyDamageTaken(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.f) return;
	// 방어력 고려(있다면)
	const float Defense = GetAttribute(TEXT("Defense"), 0.f);
	const float Final = FMath::Max(0.f, Damage - Defense);
	ApplyDamageAmount(Final);
}



// Copyright ChangJin

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraProjectileSpell.generated.h"

class AAuraProjectile;
/**
 * 
 */
UCLASS()
class AURA_API UAuraProjectileSpell : public UAuraGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<AAuraProjectile> ProjectileClass;

	UFUNCTION(BlueprintCallable)
	void SpawnProjectile(const FVector& _projectileTargetLocation);

	UFUNCTION(BlueprintCallable)
	void SetFacingTarget(const FName& FacingName, const FVector& FacingVector);

	UFUNCTION(BlueprintCallable)
	void SetProjectileTargetLocation(const FGameplayAbilityTargetDataHandle& DataHandle);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector ProjectileTargetLocation;

	FORCEINLINE void SetProjectileTargetLocation(const FVector& _location) {ProjectileTargetLocation = _location;};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;
};

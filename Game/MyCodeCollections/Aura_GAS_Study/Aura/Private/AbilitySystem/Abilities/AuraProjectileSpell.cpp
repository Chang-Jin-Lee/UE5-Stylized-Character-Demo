// Copyright ChangJin


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//UKismetSystemLibrary::PrintString(this, FString("Activate Ability C++"), true, true, FLinearColor::Yellow, 3);
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& _projectileTargetLocation)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (bIsServer == false) return;
	
	// 이제 서버에서 실행되는 코드
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		FRotator Rotation = (_projectileTargetLocation - SocketLocation).Rotation();
		Rotation.Pitch = 0.0f;
		
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotation.Quaternion());
		// TODO: Set The Projectile Rotation
		
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		// TODO: Give a Projectile a Gameplay Effect Spec for causing Damage.

		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		const FGameplayEffectSpecHandle SpecHandle =SourceASC->MakeOutgoingSpec(DamageEffectClass,GetAbilityLevel(),SourceASC->MakeEffectContext());

		FAuraGameplayTags GameplayTag =FAuraGameplayTags::Get();
		
		const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());
		
		GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Red,FString::Printf(TEXT("Damage: %f"),ScaledDamage));
		
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTag.Damage, ScaledDamage);
		Projectile->DamageEffectSpecHandle = SpecHandle;
		
		Projectile->FinishSpawning(SpawnTransform);
	}
}

void UAuraProjectileSpell::SetFacingTarget(const FName& FacingName, const FVector& FacingVector)
{
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
	{
		CombatInterface->SetFacingTarget(FacingName, FacingVector);
	}
}

void UAuraProjectileSpell::SetProjectileTargetLocation(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(DataHandle,0);
	SetProjectileTargetLocation(HitResult.Location);
}


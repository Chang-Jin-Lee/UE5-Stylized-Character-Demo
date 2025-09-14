// Copyright ChangJin

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignature, const FGameplayAbilityTargetDataHandle&, DataHandle);
/**
 * 
 */
UCLASS()
class AURA_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category="Ability|Task", meta=(DisplayName = "TartgetDataUnderMouse", HidePin="OwingAbility", DefaultToSelf = "OwingAbility", BlueprintInternalUseOnly = true))
	static UTargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwingAbility);

	UPROPERTY(BlueprintAssignable, Category="Ability|Task")
	FMouseTargetDataSignature ValidData;

private:
	virtual void Activate() override;
	void SendMouseCursorData();

	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag);
};

// Copyright ChangJin

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MotionWarpingComponent.h"
#include "CombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI,BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual int32 GetPlayerLevel();
	virtual FVector GetCombatSocketLocation();
	virtual void SetFacingTarget(const FName& FacingName, const FVector& FacingVector);
	TWeakObjectPtr<UMotionWarpingComponent> MotionWarpingComponent;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();

	virtual void Dead() = 0;
};

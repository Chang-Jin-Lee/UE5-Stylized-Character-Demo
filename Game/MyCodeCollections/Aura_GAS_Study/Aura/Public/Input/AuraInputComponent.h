// Copyright ChangJin

#pragma once

#include "CoreMinimal.h"
#include "AuraInputConfig.h"
#include "EnhancedInputComponent.h"
#include "AuraInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
public:
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UAuraInputComponent::BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InputConfig);

	for (const FAuraInputAction& InputAction : InputConfig->AbilityInputActions)
	{
		if (InputAction.InputAction && InputAction.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(InputAction.InputAction, ETriggerEvent::Started, Object, PressedFunc, InputAction.InputTag);
			}
			if (ReleasedFunc)
			{
				BindAction(InputAction.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, InputAction.InputTag);
			}
			if (HeldFunc)
			{
				BindAction(InputAction.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, InputAction.InputTag);
			}
		}
	}
}

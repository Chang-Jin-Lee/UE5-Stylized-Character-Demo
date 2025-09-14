// Copyright ChangJin

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "EnemyStatusBarWidgetController.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UEnemyStatusBarWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
};

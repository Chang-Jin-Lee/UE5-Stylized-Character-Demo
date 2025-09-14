#pragma once

#include "CoreMinimal.h"
#include "portalCommon.generated.h"

UENUM(BlueprintType)
enum class EPortalType : uint8
{
	IPt_Move UMETA(DisplayName = "Move"),
	IPt_OpenLevel UMETA(DisplayName = "OpenLevel"),
	None UMETA(DisplayName = "NONE")
};

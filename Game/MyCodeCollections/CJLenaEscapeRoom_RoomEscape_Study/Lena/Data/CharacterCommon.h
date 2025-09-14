#pragma once

#include "CoreMinimal.h"
#include "CharacterCommon.generated.h"

UENUM(BlueprintType)
enum class ECharacterStateType : uint8
{
	Cst_Alive UMETA(DisplayName = "Alive"),
	Cst_Dead UMETA(DisplayName = "Dead"),
	None UMETA(DisplayName = "NONE")
};

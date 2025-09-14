// Copyright ChangJin


#include "Interaction/CombatInterface.h"

// Add default functionality here for any ICombatInterface functions that are not pure virtual.
int32 ICombatInterface::GetPlayerLevel()
{
	return 0;
}

FVector ICombatInterface::GetCombatSocketLocation()
{
	return FVector::ZeroVector;
}

void ICombatInterface::SetFacingTarget(const FName& FacingName, const FVector& FacingVector)
{
	MotionWarpingComponent.Get()->AddOrUpdateWarpTargetFromLocation(FacingName, FacingVector);
}

// Copyright Epic Games, Inc. All Rights Reserved.


#include "ChangJinCodeStory/Public/Game/GameMode/Base_GameMode.h"

#include "Characters/PlayerCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABase_GameMode::ABase_GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Script/Engine.Blueprint'/Game/Characters/BP_PlayerCharacter.BP_PlayerCharacter'"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

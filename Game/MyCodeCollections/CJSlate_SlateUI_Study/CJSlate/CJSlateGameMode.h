// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CJSlateGameMode.generated.h"

UCLASS(minimalapi)
class ACJSlateGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACJSlateGameMode();

	virtual void BeginPlay() override;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	virtual AActor* FindPlayerStart_Implementation(AController* Player, const FString& IncomingName = L"") override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	int32 NextPlayerStartIndex;
};




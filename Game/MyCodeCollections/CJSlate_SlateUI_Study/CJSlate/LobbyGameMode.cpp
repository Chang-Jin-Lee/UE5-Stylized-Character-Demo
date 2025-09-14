// Copyright Epic Games, Inc. All Rights Reserved.

#include "LobbyGameMode.h"
#include "CJSlateCharacter.h"
#include "CJSlate/HUD/CJSlateHUD.h"
#include "CJSlatePlayerController.h"
#include "CJSlatePlayerState.h"
#include "GameFramework/PlayerStart.h"
#include "GameInstance/CJSlateGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ALobbyGameMode::ALobbyGameMode()
{
	bReplicates = true;
	PlayerControllerClass = ACJSlatePlayerController::StaticClass();
	HUDClass = ACJSlateHUD::StaticClass();
	PlayerStateClass = ACJSlatePlayerState::StaticClass();
}

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	CurrentPlayerCount++;
	
	UE_LOG(LogTemp, Warning, TEXT("[Server] PostLogin Client CurrentPlayerCount++ : %d"), CurrentPlayerCount);

	// 현재 접속한 모든 플레이어 컨트롤러에게 업데이트하게 하기
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (ACJSlatePlayerController* PlayerController = Cast<ACJSlatePlayerController>(*It))
		{
			PlayerController->Multicast_UpdatePlayerCount(CurrentPlayerCount);
		}
	}
	// 새로 접속한 플레이어에게도 업데이트
	if (ACJSlatePlayerController* PlayerController = Cast<ACJSlatePlayerController>(NewPlayer))
	{
		PlayerController->Client_UpdatePlayerCount(CurrentPlayerCount);
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	CurrentPlayerCount--;

	UE_LOG(LogTemp, Warning, TEXT("[Server] Logout Client CurrentPlayer-- : %d"), CurrentPlayerCount);

	// 현재 접속한 모든 플레이어 컨트롤러에게 업데이트 명령을 전송
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (ACJSlatePlayerController* PlayerController = Cast<ACJSlatePlayerController>(*It))
		{
			if (PlayerController != Exiting)
				PlayerController->Multicast_UpdatePlayerCount(CurrentPlayerCount);
		}
	}
}

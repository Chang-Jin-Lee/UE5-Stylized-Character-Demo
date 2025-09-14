// Copyright Epic Games, Inc. All Rights Reserved.

#include "CJSlateGameMode.h"
#include "CJSlateCharacter.h"
#include "CJSlate/HUD/CJSlateHUD.h"
#include "CJSlatePlayerController.h"
#include "CJSlatePlayerState.h"
#include "GameFramework/PlayerStart.h"
#include "GameInstance/CJSlateGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ACJSlateGameMode::ACJSlateGameMode()
{
	bReplicates = true;
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/BP_CJSlateCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PlayerControllerClass = ACJSlatePlayerController::StaticClass();
	HUDClass = ACJSlateHUD::StaticClass();
	PlayerStateClass = ACJSlatePlayerState::StaticClass();
	NextPlayerStartIndex = 0;
}

void ACJSlateGameMode::BeginPlay()
{
	Super::BeginPlay();

	//GEngine->Exec(GetWorld(), TEXT("SlateDebugger.Stop"));
	// GEngine->Exec(GetWorld(), TEXT("SlateDebugger.Start"));
	// GEngine->AddOnScreenDebugMessage(-1,5.0f, FColor::Red, FString::Printf(TEXT("g_UISettingManager.GetTitle() : %s"), *g_UISettingManager));
}

AActor* ACJSlateGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);

	if (PlayerStarts.Num() == 0)
	{
		return Super::ChoosePlayerStart_Implementation(Player);
	}

	// 순차적으로 PlayerStart 선택
	AActor* ChosenStart = PlayerStarts[NextPlayerStartIndex];

	// 다음 플레이어는 다음 인덱스의 PlayerStart 사용 (순환 설정)
	NextPlayerStartIndex = (NextPlayerStartIndex + 1) % PlayerStarts.Num();

	return ChosenStart;
}

AActor* ACJSlateGameMode::FindPlayerStart_Implementation(AController* Player, const FString& IncomingName)
{
	return ChoosePlayerStart(Player);
}

void ACJSlateGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	AActor* StartSpot = ChoosePlayerStart(NewPlayer);
	if (StartSpot)
	{
		RestartPlayerAtPlayerStart(NewPlayer, StartSpot);
	}
}
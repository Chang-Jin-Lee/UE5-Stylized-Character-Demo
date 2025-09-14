// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameGameMode.h"
#include "GameCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Player/GamePlayerController.h"

/*
 * @brief : 기본 Pawn/컨트롤러 지정
 * @details : AGameCharacter, AGamePlayerController로 기본 설정
 */
AGameGameMode::AGameGameMode()
{
	// 포트폴리오: 기본 Pawn은 C++ 클래스로 설정하고 선택 시스템이 교체/스왑 담당
	DefaultPawnClass = AGameCharacter::StaticClass();
	PlayerControllerClass = AGamePlayerController::StaticClass();
}

/*
 * @brief : 게임 시작 훅
 * @details : 초기 스폰/스왑은 PlayerController의 Subsystem이 담당
 */
void AGameGameMode::BeginPlay()
{
	Super::BeginPlay();
	// Why: 초기 스폰/스왑은 PlayerController에서 Subsystem이 담당
}

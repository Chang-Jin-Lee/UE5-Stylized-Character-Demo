// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameGameMode.h"
#include "GameCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Player/GamePlayerController.h"

/*
 * @brief : �⺻ Pawn/��Ʈ�ѷ� ����
 * @details : AGameCharacter, AGamePlayerController�� �⺻ ����
 */
AGameGameMode::AGameGameMode()
{
	// ��Ʈ������: �⺻ Pawn�� C++ Ŭ������ �����ϰ� ���� �ý����� ��ü/���� ���
	DefaultPawnClass = AGameCharacter::StaticClass();
	PlayerControllerClass = AGamePlayerController::StaticClass();
}

/*
 * @brief : ���� ���� ��
 * @details : �ʱ� ����/������ PlayerController�� Subsystem�� ���
 */
void AGameGameMode::BeginPlay()
{
	Super::BeginPlay();
	// Why: �ʱ� ����/������ PlayerController���� Subsystem�� ���
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "LenaRoom03GameMode.h"

#include "EngineUtils.h"
#include "Lena/Controller/ShooterAIController.h"
#include "Lena/GameInstanceSubSystem/LenaEnemyManager.h"
#include "Lena/GameInstanceSubSystem/LenaUIManager.h"

class ALoseController;

void ALenaRoom03GameMode::PawnKilled(TObjectPtr<APawn> PawnKilled)
{
	Super::PawnKilled(PawnKilled);

	UE_LOG(LogTemp, Warning, TEXT("A Pawn was Killed!"));
	
	APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());
	if(PlayerController != nullptr)
	{
		EndGame(false);
	}

	if(g_LenaEnemyManagerIsValid)
	{
		if(g_LenaEnemyManager->GetEnemyCount() <= 1)
		{
			EndGame(true); // 플레이어가 승리
		}
	}
}

void ALenaRoom03GameMode::EndGame(bool bIsPlayerWinner)
{
	for ( AController* Controller : TActorRange<AController>(GetWorld()))
	{
		bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
		Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
	}
}

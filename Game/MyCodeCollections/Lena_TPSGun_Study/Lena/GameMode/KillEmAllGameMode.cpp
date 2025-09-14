// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"

#include "EngineUtils.h"
#include "Lena/Controller/ShooterAIController.h"
#include "GameFramework/Controller.h"

class ALoseController;

void AKillEmAllGameMode::PawnKilled(TObjectPtr<APawn> PawnKilled)
{
	Super::PawnKilled(PawnKilled);
	
	if(TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(PawnKilled->GetController()))
	{
		EndGame(false);
	}

	for ( TObjectPtr<AShooterAIController> Controller : TActorRange<AShooterAIController>(GetWorld()))
	{
		if(!Controller->IsDead())
		{
			return;
		}
	}
	// 플레이어가 승리
	EndGame(true);
}

void AKillEmAllGameMode::EndGame(bool bIsPlayerWinner)
{
	for ( AController* Controller : TActorRange<AController>(GetWorld()))
	{
		bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
		Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
	}
}

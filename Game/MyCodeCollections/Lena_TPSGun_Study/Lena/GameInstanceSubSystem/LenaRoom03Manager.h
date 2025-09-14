// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoreFramework/GameManager/CJGameManagerBase.h"
#include "CJLibrary/ManagerRegistry/Data/LenaRoom03JsonData.h"
#include "CoreFramework/GlobalInterface/CJGlobalBase.h"
#include "Lena/Controller/ShooterPlayerController.h"
#include "ManagerRegistry/Data/LenaRoom01JsonData.h"
#include "ManagerRegistry/Data/LenaUIGuide.h"
#include "LenaRoom03Manager.generated.h"

/**
 * 
 */
UCLASS()
class LENA_API ULenaRoom03Manager : public UCJGameManagerBase
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	FORCEINLINE TArray<FLenaRoom03DataRow> GetLenaRoom03Data() { return LenaRoom03Data;}
	FORCEINLINE TArray<FLenaRoom01ItemSpawnRow> GetLenaRoom03ItemSpawnData() { return LenaRoom03ItemSpawnData;}
	FORCEINLINE TArray<FLenaUIGuide> GetLenaRoom03UIGuideData(){return LenaRoom03UIGuideData;};
	FORCEINLINE TObjectPtr<ABase_Character> GetCharacter() { return Character;}
	FORCEINLINE void SetCharacter(TObjectPtr<ABase_Character> _character) { Character = _character;}
	FORCEINLINE TObjectPtr<AShooterPlayerController> GetPlayerController() { return PlayerController;}
	FORCEINLINE void SetPlayerController(TObjectPtr<AShooterPlayerController> _controller) { PlayerController = _controller;}

	
private:
	TArray<FLenaRoom03DataRow> LenaRoom03Data;
	TArray<FLenaRoom01ItemSpawnRow> LenaRoom03ItemSpawnData;
	TArray<FLenaUIGuide> LenaRoom03UIGuideData;
	UPROPERTY()
	TObjectPtr<ABase_Character> Character;
	UPROPERTY()
	TObjectPtr<AShooterPlayerController> PlayerController;
};
#define g_LenaRoom03Manager (  GetWorld()->GetGameInstance()->GetSubsystem<ULenaRoom03Manager>() )
#define g_LenaRoom03ManagerIsValid ( GetWorld()->GetGameInstance()->GetSubsystem<ULenaRoom03Manager>() != nullptr )
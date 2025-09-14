// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoreFramework/GameManager/CJGameManagerBase.h"
#include "CJLibrary/ManagerRegistry/Data/LenaRoom01JsonData.h"
#include "CoreFramework/GlobalInterface/CJGlobalBase.h"
#include "Lena/Characters/Base_Character.h"
#include "Lena/Controller/ShooterPlayerController.h"
#include "Lena/Manager/AdvancedPlayerCameraManager.h"
#include "Lena/UI/Lena_InGameWIdget.h"
#include "ManagerRegistry/Data/LenaUIGuide.h"
#include "LenaRoom01Manager.generated.h"

/**
 * 
 */
UCLASS()
class LENA_API ULenaRoom01Manager : public UCJGameManagerBase
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	void Load();

public:
	FORCEINLINE TArray<FLenaRoom01DataRow> GetLenaRoom01Data() { return LenaRoom01Data;}
	FORCEINLINE TArray<FLenaRoom01ItemSpawnRow> GetLenaRoom01ItemSpawnData() { return LenaRoom01ItemSpawnData;}
	FORCEINLINE TArray<FLenaRoom01InteractConditionRow> GetLenaRoom01InteractConditionData() { return LenaRoom01InteractConditionData;}
	FORCEINLINE TArray<FLenaUIGuide> GetLenaRoom01UIGuideData() { return LenaRoom01UIGuideData;}
	FORCEINLINE TObjectPtr<AAdvancedPlayerCameraManager> GetCameraManager() { return CameraManager;}
	FORCEINLINE void SetCameraManager(TObjectPtr<AAdvancedPlayerCameraManager> _cameramanager) { CameraManager = _cameramanager;}
	FORCEINLINE TObjectPtr<ABase_Character> GetCharacter() { return Character;}
	FORCEINLINE void SetCharacter(TObjectPtr<ABase_Character> _character) { Character = _character;}
	FORCEINLINE TObjectPtr<AShooterPlayerController> GetPlayerController() { return PlayerController;}
	FORCEINLINE void SetPlayerController(TObjectPtr<AShooterPlayerController> _controller) { PlayerController = _controller;}
	FORCEINLINE void SetInGameWidget(TObjectPtr<ULena_InGameWIdget> _player) { InGameWidget =  _player;}
	FORCEINLINE TObjectPtr<ULena_InGameWIdget> GetInGameWidget() { return InGameWidget;}
	
private:
	TArray<FLenaRoom01DataRow> LenaRoom01Data;
	TArray<FLenaRoom01ItemSpawnRow> LenaRoom01ItemSpawnData;
	TArray<FLenaRoom01InteractConditionRow> LenaRoom01InteractConditionData;
	TArray<FLenaUIGuide> LenaRoom01UIGuideData;
	UPROPERTY()
	TObjectPtr<AAdvancedPlayerCameraManager> CameraManager;
	UPROPERTY()
	TObjectPtr<ABase_Character> Character;
	UPROPERTY()
	TObjectPtr<AShooterPlayerController> PlayerController;
	UPROPERTY()
	TObjectPtr<ULena_InGameWIdget> InGameWidget;
};
#define g_LenaRoom01Manager (  GetWorld()->GetGameInstance()->GetSubsystem<ULenaRoom01Manager>() )
#define g_LenaRoom01ManagerIsValid ( GetWorld()->GetGameInstance()->GetSubsystem<ULenaRoom01Manager>() != nullptr )
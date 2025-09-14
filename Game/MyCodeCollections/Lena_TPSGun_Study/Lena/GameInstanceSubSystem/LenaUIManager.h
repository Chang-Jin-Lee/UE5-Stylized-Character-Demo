// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoreFramework/GameManager/CJGameManagerBase.h"
#include "Lena/Controller/ShooterPlayerController.h"
#include "Lena/UI/Lena_InGameWIdget.h"
#include "ManagerRegistry/Data/LenaUIGuide.h"
#include "LenaUIManager.generated.h"
/**
 * 
 */
UCLASS()
class LENA_API ULenaUIManager : public UCJGameManagerBase
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	void LoadData(TArray<FLenaUIGuide> _uIGuideData);
	void CreateGuideWidget(bool _isStart);
	void InputModeUI();
	void InputModeGame();
	
	FORCEINLINE TArray<FLenaUIGuide> GetLenaLenaUIGuide() { return LenaUIGuideData;}
	FORCEINLINE void SetShooterPlayerController(AShooterPlayerController* _controller) { ShooterPlayerController =  _controller;}
	FORCEINLINE TObjectPtr<AShooterPlayerController> GetShooterPlayerController() { return ShooterPlayerController;}
	FORCEINLINE void SetBaseCharacter(TObjectPtr<ABase_Character> _player) { BaseCharacter =  _player;}
	FORCEINLINE TObjectPtr<ABase_Character> GetBaseCharacter() { return BaseCharacter;}
	FORCEINLINE void SetInGameWidget(TObjectPtr<ULena_InGameWIdget> _player) { InGameWidget =  _player;}
	FORCEINLINE TObjectPtr<ULena_InGameWIdget> GetInGameWidget() { return InGameWidget;}

private:
	TArray<FLenaUIGuide> LenaUIGuideData;
	TObjectPtr<AShooterPlayerController> ShooterPlayerController;
	TObjectPtr<ABase_Character> BaseCharacter;
	TObjectPtr<ULena_InGameWIdget> InGameWidget;
};
#define g_LenaUIManager (  GetWorld()->GetGameInstance()->GetSubsystem<ULenaUIManager>() )
#define g_LenaUIManaerIsValid ( GetWorld()->GetGameInstance()->GetSubsystem<ULenaUIManager>() != nullptr )
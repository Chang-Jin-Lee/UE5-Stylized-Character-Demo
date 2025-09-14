// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lena/Characters/Base_Character.h"
#include "GameFramework/PlayerController.h"
#include "Lena/UI/InteractWidget.h"
#include "Lena/UI/InventoryWidget.h"
#include "Lena/UI/Lena_InGameMenuPannel.h"
#include "ShooterPlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class LENA_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AShooterPlayerController();
	virtual void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;
	UFUNCTION()
	void UpdateCountdown();
	UFUNCTION()
	void OpenIntroScene();
	void EnableMouseClick();
	void DisableMouseClick();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	void OnToggleFlashLight(const FInputActionValue& value);
	void OnOpenMenuInGame(const FInputActionValue& value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Input")
	TObjectPtr<UInputAction> IA_MouseClick;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Input")
	TObjectPtr<UInputAction> IA_PickUpItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Input")
	TObjectPtr<UInputAction> IA_OpenMenuInGame;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Input")
	TObjectPtr<UInputAction> IA_OpenInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Input")
	TObjectPtr<UInputAction> IA_ToggleFlashLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	TObjectPtr<UInputMappingContext> IMC_Interaction;

	void HandleMouseClick();
	
	//Widget
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"), Category="Inventory")
	TObjectPtr<UInventoryWidget> InventoryWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"), Category="Inventory")
	TObjectPtr<UInteractWidget> ItemInfoWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"), Category="Inventory")
	TObjectPtr<AActor> LineTraceItem;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"), Category="Inventory")
	bool IsInWidget = false;

	UPROPERTY()
	TObjectPtr<ABase_Character> Base_Character;
	FTimerHandle TimerHandle_CheckPickUpItem;
	
public:
	UFUNCTION(BlueprintCallable)
	void InputModeUI();
	UFUNCTION(BlueprintCallable)
	void InputModeUIOnly();
	UFUNCTION(BlueprintCallable)
	void InputModeGame();
	
	void HandlePickUpItem();
	void HandleOpenInventory(const FInputActionValue& ActionValue);
	void CheckPickUpItemLine();
	UFUNCTION(BlueprintCallable)
	void CheckPickUpItemSweep();

	FORCEINLINE void SetbMenuShowed(bool _bMenuShowed) { bMenuShowed = _bMenuShowed; }
	FORCEINLINE bool GetbMenuShowed() const { return bMenuShowed; }
	
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> WinScreenClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> LoseScreenClass;
	
	UPROPERTY(EditAnywhere)
	float RestartDelay = 5.0f;
	
	FTimerHandle RestartTimer;
	FTimerHandle CountdownTimerHandle;

	void OnMouseClick();
	bool bIsClickEnabled;

	bool bFlipFlop;
	bool bMenuShowed = false;
	UPROPERTY()
	TObjectPtr<ULena_InGameMenuPannel> MenuPannel;
};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GamePlayerController.generated.h"

class UCharacterSelectionSubsystem;
class SPlayerOverlayWidget;
class SShopWidget;
class SInventoryWidget;
class UInputAction;
struct FInputActionInstance;

UCLASS()
class GAME_API AGamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Character")
	void RequestSelectCharacter(const FString& PrimaryAssetIdString);

	// 콘솔에서: CharList, CharSelect CharacterDef:AssetName
	UFUNCTION(Exec)
	void CharList();

	UFUNCTION(Exec)
	void CharSelect(const FString& PrimaryAssetIdString);

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaSeconds) override;
	void OnInteractStarted(const FInputActionInstance& Instance);
	void OnInventoryStarted(const FInputActionInstance& Instance);
	void OnCtrlPressed();
	void OnCtrlReleased();

private:
	UCharacterSelectionSubsystem* GetCharacterSubsystem() const;

	// Slate overlays
	TSharedPtr<SPlayerOverlayWidget> PlayerOverlay;
	TSharedPtr<SShopWidget> ShopUI;               // fallback (Slate)
	TSharedPtr<SInventoryWidget> InventoryUI;     // fallback (Slate)
	TSharedPtr<class SWidget> InventoryOverlayRoot; // fallback root to remove
	TSharedPtr<class SWidget> ShopOverlayRoot;      // fallback root to remove

	// UMG inventory/shop
	UPROPERTY(EditAnywhere, Category=UI)
	TSubclassOf<class UUserWidget> InventoryWidgetClass;
	UPROPERTY(EditAnywhere, Category=UI)
	TSubclassOf<class UUserWidget> ShopWidgetClass;
	TWeakObjectPtr<class UUserWidget> InventoryWidget;
	TWeakObjectPtr<class UUserWidget> ShopWidget;

	// Input actions for F/I/Ctrl (optional direct binding)
	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* InteractAction = nullptr;
	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* InventoryAction = nullptr;
	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* CtrlAction = nullptr;

	bool bUIActionsBound = false;

	// Shop tracking
	TWeakObjectPtr<AActor> CurrentShopTarget;
	float ShopClosePadding = 50.f;
};



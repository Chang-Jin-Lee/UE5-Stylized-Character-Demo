#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GamePlayerController.generated.h"

class UCharacterSelectionSubsystem;

UCLASS()
class GAME_API AGamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Character")
	void RequestSelectCharacter(const FString& PrimaryAssetIdString);

protected:
	virtual void BeginPlay() override;

private:
	UCharacterSelectionSubsystem* GetCharacterSubsystem() const;
};



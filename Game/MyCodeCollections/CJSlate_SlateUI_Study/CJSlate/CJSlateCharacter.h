// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CJSlate/HUD/CJSlateHUD.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "CJSlateCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ACJSlateCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MainMenuAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* OpenInventoryAction;

public:
	ACJSlateCharacter();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	void UpdateSpeechBubblePosition();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_UpdateMessage(const FSChatMsgInfo& newmessage);
	void SetMessage(const FSChatMsgInfo& newmessage);
	TSharedPtr<class SCJSlateSpeechBubble> SpeechBubblePtr;
	TSharedPtr<class Swidget> SpeechBubbleContainer;
	UFUNCTION(NetMulticast, Reliable,WithValidation)
	void ChangeMaterialColor(FLinearColor Color);

protected:
	/** Called for input */
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void ShowMainMenu(const FInputActionValue& Value);
	void OpenInventory(const FInputActionValue& Value);
public:
	bool bShowMainMenu = false;

protected:
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/** Returns subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/** Item */
public:
	UFUNCTION()
	void OnCharacterComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnCharacterComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
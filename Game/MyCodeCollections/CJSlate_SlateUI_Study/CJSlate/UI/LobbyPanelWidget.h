// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CJSlate/Interface/GuideWidgetInterface.h"
#include "LobbyPanelWidget.generated.h"

class UEditableText;
class UEditableTextBox;
class UCheckBox;
class UTextBlock;
class UCircularThrobber;
/**
 * 
 */
class UButton;
UCLASS()
class CJSLATE_API ULobbyPanelWidget : public UUserWidget, public IGuideWidgetInterface
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidgetOptional))
	UButton* ReturnButton;
	
	UPROPERTY(meta = (BindWidgetOptional))
	UButton* HostButton;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* JoinButton;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* ClientTravelButton;
	
	UPROPERTY(meta = (BindWidgetOptional))
	UButton* ServerTravelButton;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* QuitButton;

	UPROPERTY(meta = (BindWidgetOptional))
	UCheckBox* UseLanCheckBox;

	UPROPERTY(meta = (BindWidgetOptional))
	UEditableText* EditTextBlockJoinIPAdress;

	UPROPERTY(meta = (BindWidgetOptional))
	UEditableText* EditTextBlockSesionName;

	// 현재 접속 중인 사람의 수
	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* TextBlock_ConnectedPeople;

	UFUNCTION(BlueprintCallable)
	void Return();

	UFUNCTION()
	void CreateHost();

	UFUNCTION()
	void JoinGame();

	UFUNCTION()
	void ClientTravel();

	UFUNCTION()
	void ServerTravel();

	UFUNCTION()
	void QuitGame();

	UFUNCTION()
	void UseLANToggle(bool _bIsChecked);

	UPROPERTY()
	bool bUseLan;

	// IP 주소
	UFUNCTION()
	void IPAdressChanged(const FText& _value);
	
	UPROPERTY()
	FText IpAdress;
	
	// 입력 받은 세션 이름
	UFUNCTION()
	void SessionNameChanged(const FText& _value);

	UPROPERTY()
	FText SessionName;

	void UpdatePlayerCounts();

	UPROPERTY(meta = (BindWidgetOptional))
	UCircularThrobber* CircularThrobber_LoadingBar;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* TextBlock_Description;

	UPROPERTY(meta = (BindWidgetOptional))
	UBorder* Border_GuideBackGround;

	UPROPERTY(meta = (BindWidgetAnimOptional), Transient)
	UWidgetAnimation* WidgetAnimation_GuideFadein;

	UPROPERTY(meta = (BindWidgetAnimOptional), Transient)
	UWidgetAnimation* WidgetAnimation_GuideFadeOut;
	
	virtual UCircularThrobber* GetLoadingBar() const override { return CircularThrobber_LoadingBar; }
	virtual UTextBlock* GetDescriptionText() const override { return TextBlock_Description; }
	virtual UBorder* GetGuideBackGroundBorder() const override { return Border_GuideBackGround; }
	virtual UWidgetAnimation* GetGuideFadeInAnimation() const override { return WidgetAnimation_GuideFadein; }
	virtual UWidgetAnimation* GetGuideFadeOutAnimation() const override { return WidgetAnimation_GuideFadeOut; }

	virtual void PlayAnimation_GuideFadeOut() override;
	virtual void PlayAnimation_GuideFadeIn() override;
	virtual void TimeOut() override;
};

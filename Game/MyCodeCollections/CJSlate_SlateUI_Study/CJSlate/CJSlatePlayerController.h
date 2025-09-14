// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CJSlate/HUD/CJSlateHUD.h"
#include "InputAction.h"
#include "GameFramework/PlayerController.h"
#include "CJSlatePlayerController.generated.h"

class ALockActor;
struct FItemActorInfo;
class AItemActor;
class AButtonActor;
class UInGamePanelWidget;

/**
 * 
 */
UCLASS()
class CJSLATE_API ACJSlatePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void ShowMenu() const;
	void ExitMenu() const;

	void OpenInventory();
	void CloseInventory();

	virtual void Tick(float DeltaSeconds) override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<class AAnswerManager> BeaconManager;

	UFUNCTION(BlueprintCallable)
	void HandleMouseHover();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ClickButton(AButtonActor* ButtonActor, const TArray<int32>& IsHoldings);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UInGamePanelWidget> HUDPannelWidgetptr;

	UInGamePanelWidget* GetHUDPanel() const;
	
	// Session
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RequestCreateSession(int32 NumPublicConnections, const FString& _SessionName, int32 SessionCode,bool IsLANMatch);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ServerTravel(const FString& _levelname);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RestartLevel();

	// 남은 시간 보여주기
	// 포탈에서 이동하는 때 쓰임
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ServerTravelWithDealy(const FString& _levelname, float delay);
	
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multicast_VoteServerTravel();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_VoteCountAdd();

	// 이동할 레벨의 이름이 /Game/Level 폴더에 있는 레벨인지 아닌지 판단.
	static bool IsLevelValid(const FString& LevelName);

	void ExecuteAfterTimer(FString _levelname);

	//** 남은 시간에 대한 UI를 클라이언트에게 뿌려줄 함수.
	// 남은 시간은 서버에서만 계산함. */
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void PopUpUIRemainTime(float _time);

	UPROPERTY()
	bool m_bCameraMoved = false;	// 카메라가 캐릭터에 있는지 이동되어 있는지를 판단하는 변수

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float m_fBlendTime = 1.0f;

	UPROPERTY()
	ALockActor* m_aCurrentLockActor = nullptr; // 카메라에서 현재 보고 있는 자물쇠 액터

	//* 자물쇠에서 버튼을 클릭하면 서버에게 자물쇠 공, 버튼, 다이얼을 움직이게 요청하는 함수
	//*/
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_LockMoveFromStart(ALockActor* TargetActor, FVector InputVector, int32 SelectedWheelIndex);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_LockMoveFromStart(ALockActor* TargetActor, FVector InputVector, int32 SelectedWheelIndex);
	
private:
	FTimerHandle CountdownTimerHandle;
	UPROPERTY(EditDefaultsOnly, Replicated)
	float m_fremainingTime;
	
	UPROPERTY(EditDefaultsOnly, Replicated)
	float m_fvotePlayerCount = 0;

public:
	// 접속한 플레이어 수
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multicast_UpdatePlayerCount(int32 PlayerCount);

	UFUNCTION(Client, Reliable, WithValidation)
	void Client_UpdatePlayerCount(int32 PlayerCount);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_GetPlayerCount();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multicast_GetPlayerCount(int32 PlayerCount);
	
	int32 CurrentPlayerCount = 1;
	
	//** Item 관련*/
	UPROPERTY(EditAnywhere)
	TArray<FItemActorInfo> m_fItemActorInfo;
	
	// 아이템 습득 관련
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ItemPickUp(AItemActor* _item);
	
	UFUNCTION(Client, Reliable, WithValidation)
	void Client_ItemPickUp(FItemActorInfo _iteminfo);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ItemDrop(FItemActorInfo _iteminfo);

	UFUNCTION(Client, Reliable, WithValidation)
	void Client_ItemDrop(FItemActorInfo _iteminfo);
};

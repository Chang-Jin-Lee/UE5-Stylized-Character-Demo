// Fill out your copyright notice in the Description page of Project Settings.


#include "CJSlatePlayerController.h"

#include "CJSlateCharacter.h"
#include "Item/ButtonActor.h"
#include "Camera/CameraComponent.h"
#include "Components/TextBlock.h"
#include "Components/WidgetInteractionComponent.h"
#include "GameInstance/CJSlateGameInstance.h"
#include "HUD/LobbyHUD.h"
#include "Item/ItemActor.h"
#include "Item/LockActor.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "UI/InGamePanelWidget.h"
#include "UI/LobbyPanelWidget.h"

class UCJSlateGameInstance;

void ACJSlatePlayerController::ShowMenu() const
{
	if (ACJSlateHUD* HUD = Cast<ACJSlateHUD>(GetHUD()))
	{
		HUD->ShowMenu();
	}
}

void ACJSlatePlayerController::ExitMenu() const
{
	if (ACJSlateHUD* HUD = Cast<ACJSlateHUD>(GetHUD()))
	{
		HUD->ExitMenu();
	}
}

void ACJSlatePlayerController::OpenInventory()
{
	if (ACJSlateHUD* hud = Cast<ACJSlateHUD>(GetHUD()))
	{
		hud->OpenInventory();
		bShowMouseCursor = true;
		SetInputMode(FInputModeGameAndUI());
	}
}

void ACJSlatePlayerController::CloseInventory()
{
	if (ACJSlateHUD* hud = Cast<ACJSlateHUD>(GetHUD()))
	{
		hud->DestoryInventory();
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
	}
}

void ACJSlatePlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	HandleMouseHover();

	if (GetWorld()->GetTimerManager().IsTimerActive(CountdownTimerHandle))
	{
		m_fremainingTime = GetWorld()->GetTimerManager().GetTimerRemaining(CountdownTimerHandle);
		UE_LOG(LogTemp, Warning, TEXT("m_fremainingTime :  %f"), m_fremainingTime);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("HitActor %f"), m_fremainingTime));
		PopUpUIRemainTime(m_fremainingTime);
	}
}

void ACJSlatePlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ACJSlatePlayerController, m_fremainingTime);
}

void ACJSlatePlayerController::HandleMouseHover()
{
	// 마우스 위치 좌표를 기준으로.
	// float MouseX, MouseY;
	// this->GetMousePosition(MouseX, MouseY);
	// FVector2D ScreenPosition(MouseX, MouseY);

	// 카메라가 이동된 다음에는 이 로직이 실행됨.
	if (m_bCameraMoved)
	{
		if (WasInputKeyJustPressed(EKeys::RightMouseButton))
		{
			SetViewTargetWithBlend(GetCharacter(), m_fBlendTime);
			GetCharacter()->EnableInput(this);
			GetCharacter()->SetActorHiddenInGame(false);
			m_aCurrentLockActor = nullptr;
			m_bCameraMoved = false;
			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());
			return;
		}
		
		const FGameplayTag LockTag = FGameplayTag::RequestGameplayTag(FName("Item.Lock"));
		const FGameplayTag DirectionalTag = FGameplayTag::RequestGameplayTag(FName("Item.Lock.Directional"));
		const FGameplayTag CombinationTag = FGameplayTag::RequestGameplayTag(FName("Item.Lock.Combination"));
		const FGameplayTag CombinationAlphabetTag = FGameplayTag::RequestGameplayTag(FName("Item.Lock.Combination.Alphabet"));
		const FGameplayTag CombinationNumberTag = FGameplayTag::RequestGameplayTag(FName("Item.Lock.Combination.Number"));
		const FGameplayTag ButtonTag = FGameplayTag::RequestGameplayTag(FName("Item.Lock.Button"));
		
		const FGameplayTagContainer& ItemTags = m_aCurrentLockActor->m_fitemInfo.ItemTag;
					
		if (ItemTags.HasTag(LockTag))
		{
			if (WasInputKeyJustPressed(EKeys::LeftMouseButton))
			{
				if (m_aCurrentLockActor->LockDirectionWidgetInteraction)
				{
					m_aCurrentLockActor->LockDirectionWidgetInteraction->PressPointerKey(EKeys::LeftMouseButton);
					m_aCurrentLockActor->LockDirectionWidgetInteraction->ReleasePointerKey(EKeys::LeftMouseButton);
				}
			}
		}
		return;
	}

	ACJSlateCharacter* ch = Cast<ACJSlateCharacter>(GetCharacter());
	if (ch == nullptr) return;
	
	FVector ForwardVector = ch->GetFollowCamera()->GetForwardVector();
	FVector Start = ch->GetFollowCamera()->GetComponentLocation();
	// FVector Start = ch->GetFollowCamera()->GetComponentLocation() + (ForwardVector * 300.0f);
	FVector End = Start + (ForwardVector * 600.0f); // Adjust the length of the ray as needed
	//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 10.0f, 0, 1.0f);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	// Params.AddIgnoredActor(GetPawn());
	Params.AddIgnoredActor(this);

	if (HUDPannelWidgetptr == nullptr)
		HUDPannelWidgetptr = GetHUDPanel();
		
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
	{
		//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 10.0f, 0, 1.0f);
		AActor* HitActor = HitResult.GetActor();
		if (WasInputKeyJustPressed(EKeys::LeftMouseButton))
		{
			if(HitActor)
			{
				const FGameplayTag ButtonTag = FGameplayTag::RequestGameplayTag(FName("Item.Button"));
				//UE_LOG(LogTemp, Log, TEXT("CorrectAnswer: %s"), *CurrentAnswer);
				// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("HitActor %s"), *HitActor->GetName()));
				if (AButtonActor* ButtonActor = Cast<AButtonActor>(HitActor))
				{
					if (ButtonActor->m_fitemInfo.ItemTag.HasTag(ButtonTag))
					{
						TArray<int32> IsHoldings;
						for (auto Item : m_fItemActorInfo)
							IsHoldings.Add(Item.ItemID);
						
						Server_ClickButton(ButtonActor, IsHoldings);
					}
				}
				else if ( ALockActor* LockActor = Cast<ALockActor>(HitActor) )
				{
					const FGameplayTag LockTag = FGameplayTag::RequestGameplayTag(FName("Item.Lock"));
					// const FGameplayTag DirectionalTag = FGameplayTag::RequestGameplayTag(FName("Item.Lock.Directional"));
					// const FGameplayTag CombinationTag = FGameplayTag::RequestGameplayTag(FName("Item.Lock.Combination"));
					// const FGameplayTag CombinationAlphabetTag = FGameplayTag::RequestGameplayTag(FName("Item.Lock.Combination.Alphabet"));
					// const FGameplayTag CombinationNumberTag = FGameplayTag::RequestGameplayTag(FName("Item.Lock.Combination.Number"));
					// const FGameplayTag ButtonTag = FGameplayTag::RequestGameplayTag(FName("Item.Lock.Button"));

					const FGameplayTagContainer& ItemTags = LockActor->m_fitemInfo.ItemTag;
					
					if (ItemTags.HasTag(LockTag))
					{
						if (m_bCameraMoved == false)
						{
							SetViewTargetWithBlend(LockActor, m_fBlendTime);
							GetCharacter()->DisableInput(this);
							GetCharacter()->SetActorHiddenInGame(true);
							m_aCurrentLockActor = LockActor;
							m_bCameraMoved = true;
							bShowMouseCursor = true;
							SetInputMode(FInputModeGameAndUI());
						}
						// UCameraComponent* camera = Cast<ACJSlateCharacter>(GetCharacter())->GetFollowCamera();
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("LockActor %s"), *LockActor->GetName()));
					}
				}
			}
		}

		if (WasInputKeyJustPressed(EKeys::RightMouseButton))
		{
			if(HitActor)
			{
				//UE_LOG(LogTemp, Log, TEXT("CorrectAnswer: %s"), *CurrentAnswer);
				// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("HitActor %s"), *HitActor->GetName()));
				if (AItemActor* ItemActor = Cast<AItemActor>(HitActor))
				{
					const FGameplayTag InventoryTag = FGameplayTag::RequestGameplayTag(FName("Item.Inventory"));
					const FGameplayTag WorldTag = FGameplayTag::RequestGameplayTag(FName("Item.World"));
					const FGameplayTag PortalTag = FGameplayTag::RequestGameplayTag(FName("Item.World.Portal"));

					const FGameplayTagContainer& ItemTags = ItemActor->m_fitemInfo.ItemTag;
					
					if (ItemTags.HasTag(InventoryTag))
					{
						Server_ItemPickUp(ItemActor);
						if (m_fItemActorInfo.Num() > 0)
							GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("아이템을 담았습니다 : %s"), *ItemActor->m_fitemInfo.ItemName));
					}
					else if (ItemTags.HasTag(WorldTag))
					{
						if (ItemTags.HasTag(PortalTag))
						{
							// Server_ServerTravel("Room02");
							FString PortalLevel = ItemActor->m_fitemInfo.PortalLevel;
							if (IsLevelValid(PortalLevel))
								Server_ServerTravelWithDealy(PortalLevel, 5.0f);
						}
					}
				}
			}
		}
		// DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.0f, 0, 1.0f);
		if (HitActor && HUDPannelWidgetptr)
		{
			if (AItemActor* ItemActor = Cast<AItemActor>(HitActor))
			{
				HUDPannelWidgetptr->Interact(ItemActor->m_fitemInfo.ItemName);
			}
		}
	}
	else
	{
		if (HUDPannelWidgetptr)
			HUDPannelWidgetptr->InteractDone();
	}
}

void ACJSlatePlayerController::Server_ClickButton_Implementation(AButtonActor* ButtonActor, const TArray<int32>& IsHoldings)
{
	if (ButtonActor)
	{
		ButtonActor->ClickButton();
		ButtonActor->HandleClick(IsHoldings);
	}
}

bool ACJSlatePlayerController::Server_ClickButton_Validate(AButtonActor* ButtonActor, const TArray<int32>& IsHoldings)
{
	return true;
}

UInGamePanelWidget* ACJSlatePlayerController::GetHUDPanel() const
{
	if (ACJSlateHUD* hud = Cast<ACJSlateHUD>(GetHUD()))
	{
		if (hud->m_wHUDPanelWidget)
			return hud->m_wHUDPanelWidget;
	}
	return nullptr;
}

// 서버에서 돌릴 타이머가 끝났을 때 실행될 함수
void ACJSlatePlayerController::ExecuteAfterTimer(FString _levelname)
{
	if (HasAuthority())
	{
		GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
		FString t = _levelname + FString("?listen");
		GetWorld()->ServerTravel(t, TRAVEL_Absolute);
	}
}

void ACJSlatePlayerController::Multicast_LockMoveFromStart_Implementation(ALockActor* TargetActor, FVector InputVector,
	int32 SelectedWheelIndex)
{
	if (!HasAuthority())
		TargetActor->MoveFromStart(InputVector, SelectedWheelIndex);
}

void ACJSlatePlayerController::Server_LockMoveFromStart_Implementation(ALockActor* TargetActor, FVector InputVector, int32 SelectedWheelIndex)
{
	if (HasAuthority())
	{
		TargetActor->MoveFromStart(InputVector, SelectedWheelIndex);
		Multicast_LockMoveFromStart(TargetActor, InputVector, SelectedWheelIndex);
	}
}

bool ACJSlatePlayerController::Server_LockMoveFromStart_Validate(ALockActor* TargetActor, FVector InputVector, int32 SelectedWheelIndex)
{
	if (InputVector == FVector(-1, -1, -1) && SelectedWheelIndex == -1)
		return false;
	return true;
}

void ACJSlatePlayerController::PopUpUIRemainTime_Implementation(float _time)
{
	if (ACJSlateHUD* hud = Cast<ACJSlateHUD>(GetHUD()))
	{
		if (hud->m_wHUDPanelWidget)
		{
			hud->m_wHUDPanelWidget->PopUp(FText::FromString(FString::Printf(TEXT("Remain Time : %.0f"), _time)));
		}
	}
}

bool ACJSlatePlayerController::PopUpUIRemainTime_Validate(float _time)
{
	return true;
}

void ACJSlatePlayerController::Server_ServerTravelWithDealy_Implementation(const FString& _levelname, float delay)
{
	if (delay <= 0) return;

	GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
	FTimerDelegate timerDelegate = FTimerDelegate::CreateUObject( this, &ACJSlatePlayerController::ExecuteAfterTimer, _levelname);
	GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, timerDelegate, delay, false);
}

bool ACJSlatePlayerController::Server_ServerTravelWithDealy_Validate(const FString& _levelname, float delay)
{
	return !_levelname.IsEmpty() && HasAuthority() && delay > 0;
}

void ACJSlatePlayerController::Multicast_VoteServerTravel_Implementation()
{
	
}

bool ACJSlatePlayerController::Multicast_VoteServerTravel_Validate()
{
	return !HasAuthority();
}

void ACJSlatePlayerController::Server_VoteCountAdd_Implementation()
{
	// m_fvotePlayerCount += 1;
	// if (m_fvotePlayerCount >= CurrentPlayerCount)
	// {
	// 	Server_ServerTravelWithDealy();
	// }
}

bool ACJSlatePlayerController::Server_VoteCountAdd_Validate()
{
	return HasAuthority();
}

bool ACJSlatePlayerController::IsLevelValid(const FString& LevelName)
{
	FString BasePath = FPaths::ProjectContentDir() + TEXT("Level/");
	FString SearchPath = BasePath + TEXT("*.umap");
    
	TArray<FString> FoundFiles;
	IFileManager::Get().FindFilesRecursive(FoundFiles, *BasePath, TEXT("*.umap"), true, false, false);

	for (const FString& FilePath : FoundFiles)
	{
		FString FoundLevelName = FPaths::GetBaseFilename(FilePath);
		if (FoundLevelName.Equals(LevelName, ESearchCase::IgnoreCase))
		{
			return true;
		}
	}

	return false;
}

void ACJSlatePlayerController::Client_ItemDrop_Implementation(FItemActorInfo _iteminfo)
{
	if (!HasAuthority())
	{
		for (int32 i = 0; i < m_fItemActorInfo.Num(); i++)
		{
			FItemActorInfo info = m_fItemActorInfo[i];
			if (info.ItemID == _iteminfo.ItemID)
			{
				if (ACJSlateHUD* hud = Cast<ACJSlateHUD>(GetHUD()))
				{
					if (hud->m_wHUDPanelWidget)
					{
						hud->m_wHUDPanelWidget->PopUp(FText::FromString(FString("Drop Item ") + _iteminfo.ItemName));
					}
				}
				if (info.ItemCount == 1)
				{
					m_fItemActorInfo.RemoveAt(i);
				}
				else
				{
					m_fItemActorInfo[i].ItemCount--;
				}
			}
		}
	}
}

bool ACJSlatePlayerController::Client_ItemDrop_Validate(FItemActorInfo _iteminfo)
{
	return true;
}

// 서버에서 해당 아이템이 이 플레이어가 주울 수 있는지,
// 혹은 주인인지 등의 로직을 여기서 추가해야함.
// 지금은 아이템 클릭하면 클릭한 사람이 바로 주울 수 있음.
void ACJSlatePlayerController::Server_ItemPickUp_Implementation(AItemActor* _item)
{
	if (HasAuthority())
	{
		Client_ItemPickUp(_item->m_fitemInfo);
		_item->DestroyItem();
	}
}

bool ACJSlatePlayerController::Server_ItemPickUp_Validate(AItemActor* _item)
{
	return true;
}

void ACJSlatePlayerController::Server_ItemDrop_Implementation(FItemActorInfo _iteminfo)
{
	FVector ForwardVector = GetCharacter()->GetActorForwardVector();
	FVector CurrentLocation = GetCharacter()->GetActorLocation();
	FVector TargetLocation = CurrentLocation + ForwardVector * 25;
	UClass* actorclass = LoadObject<UClass>(NULL, TEXT("/Game/Blueprint/Items/BP_Item.BP_Item_C"));
	if (AItemActor* spawnedactor = Cast<AItemActor>(GetWorld()->SpawnActor(actorclass, &TargetLocation)))
	{
		spawnedactor->m_fitemInfo = _iteminfo;
		// 아이템 정보가 업데이트가 안된다면 강제로 실행
		// spawnedactor->ForceNetUpdate();
	}
	Client_ItemDrop(_iteminfo);
}

bool ACJSlatePlayerController::Server_ItemDrop_Validate(FItemActorInfo _iteminfo)
{
	return true;
}

void ACJSlatePlayerController::Client_ItemPickUp_Implementation(FItemActorInfo _iteminfo)
{
	if (!HasAuthority())
	{
		if (ACJSlateHUD* hud = Cast<ACJSlateHUD>(GetHUD()))
		{
			if (hud->m_wHUDPanelWidget)
			{
				hud->m_wHUDPanelWidget->PopUp(FText::FromString(FString("Pick Up Item ") + _iteminfo.ItemName));
			}
		}

		bool IsPresence = false;
		for (int32 i = 0; i < m_fItemActorInfo.Num(); i++)
		{
			FItemActorInfo info = m_fItemActorInfo[i];
			if (info.ItemID == _iteminfo.ItemID)
			{
				m_fItemActorInfo[i].ItemCount++;
				IsPresence = true;
				break;
			}
		}
		if (!IsPresence)
			m_fItemActorInfo.Add(_iteminfo);
	}
}

bool ACJSlatePlayerController::Client_ItemPickUp_Validate(FItemActorInfo _iteminfo)
{
	return true;
}

void ACJSlatePlayerController::Server_ServerTravel_Implementation(const FString& _levelname)
{
	if (HasAuthority())
	{
		FString t = _levelname + FString("?listen");
		GetWorld()->ServerTravel(t, TRAVEL_Absolute);
	}
}

bool ACJSlatePlayerController::Server_ServerTravel_Validate(const FString& _levelname)
{
	return true;
}

void ACJSlatePlayerController::Server_RestartLevel_Implementation()
{
	if (HasAuthority())
	{
		FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);
		UGameplayStatics::OpenLevel(GetWorld(), FName(*CurrentLevelName), true, TEXT("?listen"));
	}
}

bool ACJSlatePlayerController::Server_RestartLevel_Validate()
{
	return true;
}

void ACJSlatePlayerController::Client_UpdatePlayerCount_Implementation(int32 PlayerCount)
{
	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("[Client] Client_UpdatePlayerCount_Implementation() 호출됨! 서버로부터 요청을 받음. CurrentPlayerCount : %d"), PlayerCount);
		CurrentPlayerCount = PlayerCount;
		if (ALobbyHUD* hud = Cast<ALobbyHUD>(GetHUD()))
		{
			if (hud->HUDPannelWidget && hud->HUDPannelWidget->TextBlock_ConnectedPeople)
				hud->HUDPannelWidget->TextBlock_ConnectedPeople->SetText(FText::FromString(FString::FromInt(PlayerCount)));
		}
	}
}

bool ACJSlatePlayerController::Client_UpdatePlayerCount_Validate(int32 PlayerCount)
{
	return true;
}

void ACJSlatePlayerController::Multicast_GetPlayerCount_Implementation(int32 PlayerCount)
{
	UE_LOG(LogTemp, Warning, TEXT("[Client] Multicast_GetPlayerCount_Implementation() 호출됨! 서버로부터 요청을 받음. PlayerCount : %d"), PlayerCount);
	if (!HasAuthority())
	{
		ALobbyHUD* hud = Cast<ALobbyHUD>(GetHUD());
		CurrentPlayerCount = PlayerCount;
		if (hud)
		{
			if (hud->HUDPannelWidget && hud->HUDPannelWidget->TextBlock_ConnectedPeople)
				hud->HUDPannelWidget->TextBlock_ConnectedPeople->SetText(FText::FromString(FString::FromInt(PlayerCount)));
		}
	}
}

bool ACJSlatePlayerController::Multicast_GetPlayerCount_Validate(int32 PlayerCount)
{
	return true;
}

void ACJSlatePlayerController::Server_GetPlayerCount_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("[Server] Server_GetPlayerCount_Implementation() 호출됨! 클라이언트 요청을 받음. CurrentPlayerCount : %d"), CurrentPlayerCount);
	Multicast_GetPlayerCount(CurrentPlayerCount);
}

bool ACJSlatePlayerController::Server_GetPlayerCount_Validate()
{
	return true;
}

void ACJSlatePlayerController::Multicast_UpdatePlayerCount_Implementation(int32 PlayerCount)
{
	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("[Client] Multicast_UpdatePlayerCount_Implementation() 호출됨! 서버로부터 요청을 받음. CurrentPlayerCount : %d"), PlayerCount);
		CurrentPlayerCount = PlayerCount;
		if (ALobbyHUD* hud = Cast<ALobbyHUD>(GetHUD()))
		{
			if (hud->HUDPannelWidget && hud->HUDPannelWidget->TextBlock_ConnectedPeople)
				hud->HUDPannelWidget->TextBlock_ConnectedPeople->SetText(FText::FromString(FString::FromInt(PlayerCount)));
		}
	}
}

bool ACJSlatePlayerController::Multicast_UpdatePlayerCount_Validate(int32 PlayerCount)
{
	return true;
}

void ACJSlatePlayerController::Server_RequestCreateSession_Implementation(int32 NumPublicConnections, const FString& _SessionName, int32 SessionCode,
                                                                          bool IsLANMatch)
{
	UE_LOG(LogTemp, Warning, TEXT("[Server] Server_RequestCreateSession() 호출됨! 클라이언트 요청을 받음."));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("[Server] Server_RequestCreateSession() 호출됨!"));
	// GetWorld()->ServerTravel(FString("Room01?listen"), true);
	
	if (UCJSlateGameInstance* GI = Cast<UCJSlateGameInstance>(GetGameInstance()))
	{
		GI->CreateSession(5, _SessionName, 54321, false);
	}
}

bool ACJSlatePlayerController::Server_RequestCreateSession_Validate(int32 NumPublicConnections, const FString& _SessionName, int32 SessionCode,
	bool IsLANMatch)
{
	return true;
}

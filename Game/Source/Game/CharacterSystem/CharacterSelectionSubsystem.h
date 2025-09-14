#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/StreamableManager.h"
#include "CharacterSelectionSubsystem.generated.h"

class UCharacterDefinition;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterSelected, UCharacterDefinition*, CharacterDef);

/*
 * @brief : 캐릭터 선택/로드/스폰을 담당하는 GameInstanceSubsystem
 * @details :
 * 	- 게임 전역 수명 주기와 함께 동작하여 레벨 전환에도 상태 유지
 * 	- 데이터 주도형 에셋을 비동기 로드하고 선택 내용을 저장/복원
 */
UCLASS()
class GAME_API UCharacterSelectionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// 캐릭터 정의 전체 목록을 비동기 스캔 후 캐시
	UFUNCTION(BlueprintCallable, Category="Character")
	void ScanCharacterAssets();

	// PrimaryAssetId 문자열로 선택
	UFUNCTION(BlueprintCallable, Category="Character")
	void SelectCharacterById(const FString& PrimaryAssetIdString);

	// 모든 캐릭터 PrimaryAssetId 문자열 나열
	UFUNCTION(BlueprintCallable, Category="Character")
	void GetAllCharacterIds(UPARAM(ref) TArray<FString>& OutIds) const;

	// 현재 선택된 정의 반환
	UFUNCTION(BlueprintPure, Category="Character")
	UCharacterDefinition* GetSelectedCharacter() const { return SelectedCharacter.Get(); }

	// 현재 선택 이벤트 (UI 바인딩 용)
	UPROPERTY(BlueprintAssignable, Category="Character")
	FOnCharacterSelected OnCharacterSelected;

	// 현재 선택 캐릭터로 플레이어를 스폰/스왑
	UFUNCTION(BlueprintCallable, Category="Character")
	APawn* SpawnOrSwapPlayer(UWorld* World, AController* Controller);

private:
	// 에셋 스트리머: 비동기 로드 관리
	FStreamableManager Streamable;

	// 선택된 정의 소프트 레퍼런스 보관 (가비지 루트 방지 위해 하드 참조 유지 가능)
	TWeakObjectPtr<UCharacterDefinition> SelectedCharacter;

	// 스캔된 에셋 ID 목록 캐시
	TArray<FPrimaryAssetId> CachedCharacterAssetIds;

	void LoadSavedSelection();
	void SaveSelection(const FPrimaryAssetId& AssetId);
};



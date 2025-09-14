#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/StreamableManager.h"
#include "CharacterSelectionSubsystem.generated.h"

class UCharacterDefinition;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterSelected, UCharacterDefinition*, CharacterDef);

/*
 * @brief : ĳ���� ����/�ε�/������ ����ϴ� GameInstanceSubsystem
 * @details :
 * 	- ���� ���� ���� �ֱ�� �Բ� �����Ͽ� ���� ��ȯ���� ���� ����
 * 	- ������ �ֵ��� ������ �񵿱� �ε��ϰ� ���� ������ ����/����
 */
UCLASS()
class GAME_API UCharacterSelectionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// ĳ���� ���� ��ü ����� �񵿱� ��ĵ �� ĳ��
	UFUNCTION(BlueprintCallable, Category="Character")
	void ScanCharacterAssets();

	// PrimaryAssetId ���ڿ��� ����
	UFUNCTION(BlueprintCallable, Category="Character")
	void SelectCharacterById(const FString& PrimaryAssetIdString);

	// ��� ĳ���� PrimaryAssetId ���ڿ� ����
	UFUNCTION(BlueprintCallable, Category="Character")
	void GetAllCharacterIds(UPARAM(ref) TArray<FString>& OutIds) const;

	// ���� ���õ� ���� ��ȯ
	UFUNCTION(BlueprintPure, Category="Character")
	UCharacterDefinition* GetSelectedCharacter() const { return SelectedCharacter.Get(); }

	// ���� ���� �̺�Ʈ (UI ���ε� ��)
	UPROPERTY(BlueprintAssignable, Category="Character")
	FOnCharacterSelected OnCharacterSelected;

	// ���� ���� ĳ���ͷ� �÷��̾ ����/����
	UFUNCTION(BlueprintCallable, Category="Character")
	APawn* SpawnOrSwapPlayer(UWorld* World, AController* Controller);

private:
	// ���� ��Ʈ����: �񵿱� �ε� ����
	FStreamableManager Streamable;

	// ���õ� ���� ����Ʈ ���۷��� ���� (������ ��Ʈ ���� ���� �ϵ� ���� ���� ����)
	TWeakObjectPtr<UCharacterDefinition> SelectedCharacter;

	// ��ĵ�� ���� ID ��� ĳ��
	TArray<FPrimaryAssetId> CachedCharacterAssetIds;

	void LoadSavedSelection();
	void SaveSelection(const FPrimaryAssetId& AssetId);
};



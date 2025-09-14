#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UObject/PrimaryAssetId.h"
#include "CharacterDefinition.generated.h"

class USkeletalMesh;

/*
 * @brief : ĳ���� ������ ������ �ֵ������� �����ϴ� Primary Data Asset
 * @details :
 * 	- ��Ÿ�ӿ� �����ϰ� �ε� �����ϸ� ���¸����� Ȯ��
 * 	- C++ �������� ���� �ű� ĳ���� �߰� ����
 */
UCLASS(BlueprintType)
class GAME_API UCharacterDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UCharacterDefinition();

	/*
	 * @brief : �������� ǥ�� �ĺ���
	 * @details : ����/��Ʈ��ũ �ĺ��� PrimaryAssetId ��� ����
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character", meta = (Tooltip = "�������� ���� ������ �ĺ��� (������ PrimaryAssetId ���)"))
	FName CharacterId;

	/*
	 * @brief : ���� �� ������ �� Ŭ����
	 * @details : ��� ������ �⺻ AGameCharacter ���� �� Mesh/Anim���� ����ȭ
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character", meta = (Tooltip = "���� �� ������ �� Ŭ���� (�Ļ� Character ����)"))
	TSoftClassPtr<APawn> PawnClass;

	/*
	 * @brief : UI/������� ��ǥ SkeletalMesh
	 * @details : ���� �����̸� �����/������ �뵵
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character", meta = (Tooltip = "������ �Ǵ� UI�� ��ǥ SkeletalMesh"))
	TSoftObjectPtr<USkeletalMesh> PreviewMesh;

	/*
	 * @brief : PrimaryAsset Ÿ�� ����
	 * @details : ������/��Ÿ�ӿ��� �ϰ��� �׷� ����
	 */
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};



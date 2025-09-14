#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UObject/PrimaryAssetId.h"
#include "CharacterDefinition.generated.h"

class USkeletalMesh;

/*
 * @brief : 캐릭터 선택을 데이터 주도형으로 정의하는 Primary Data Asset
 * @details :
 * 	- 런타임에 안전하게 로드 가능하며 에셋만으로 확장
 * 	- C++ 재컴파일 없이 신규 캐릭터 추가 가능
 */
UCLASS(BlueprintType)
class GAME_API UCharacterDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UCharacterDefinition();

	/*
	 * @brief : 가독성용 표시 식별자
	 * @details : 저장/네트워크 식별은 PrimaryAssetId 사용 권장
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character", meta = (Tooltip = "가독성을 위한 선택형 식별자 (저장은 PrimaryAssetId 사용)"))
	FName CharacterId;

	/*
	 * @brief : 선택 시 스폰할 폰 클래스
	 * @details : 비어 있으면 기본 AGameCharacter 스폰 후 Mesh/Anim으로 차별화
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character", meta = (Tooltip = "선택 시 스폰할 폰 클래스 (파생 Character 권장)"))
	TSoftClassPtr<APawn> PawnClass;

	/*
	 * @brief : UI/프리뷰용 대표 SkeletalMesh
	 * @details : 선택 사항이며 썸네일/프리뷰 용도
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character", meta = (Tooltip = "프리뷰 또는 UI용 대표 SkeletalMesh"))
	TSoftObjectPtr<USkeletalMesh> PreviewMesh;

	/*
	 * @brief : PrimaryAsset 타입 지정
	 * @details : 에디터/런타임에서 일관된 그룹 관리
	 */
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};



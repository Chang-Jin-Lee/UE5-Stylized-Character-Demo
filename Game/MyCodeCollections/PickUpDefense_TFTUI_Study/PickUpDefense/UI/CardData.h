#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"  // DataTable 사용을 위해 포함
#include "CardData.generated.h"

/**
 * 카드 데이터 구조체
 */
USTRUCT(BlueprintType)
struct FCardData : public FTableRowBase
{
	GENERATED_BODY()

	// 카드 이미지 경로
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardData")
	FString TexturePath;

	// 카드에 대응하는 메시 경로
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardData")
	FString MeshPath;

	// 카드의 시너지 (타입 목록)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardData")
	TArray<FString> Synergies;

	// 카드 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardData")
	FString Name;

	// 카드 가격
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardData")
	FString Price;
};

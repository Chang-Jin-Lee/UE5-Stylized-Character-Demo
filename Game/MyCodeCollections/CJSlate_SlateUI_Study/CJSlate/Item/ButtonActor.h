// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CJSlate/Interface/MoveInterface.h"
#include "CJSlate/Item/ItemActor.h"
#include "GameFramework/Actor.h"
#include "ButtonActor.generated.h"

class UWidgetComponent;
class AAnswerManager;
class ADoorActor;

UCLASS()
class CJSLATE_API AButtonActor : public AItemActor, public IMoveInterface
{
	GENERATED_BODY()

public:
	AButtonActor();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Move")
	FMoveData MoveData;

	UFUNCTION()
	virtual void HandleEventProgress(float Value) override;

	virtual const FMoveData& GetMoveData() const override { return MoveData; }

	// 이 버튼이 몇 번 버튼인지 확인하기 위함
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Button")
	int32 m_iButtonID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Button")
	TArray<TObjectPtr<AActor>> m_aTargetActors;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button")
	TMap<int32, int32> RequiredKeyItemIDs;	// 문을 열기 위해 필요한 열쇠의 ItemID, 수량

	UFUNCTION(NetMulticast, reliable)
	void ClickButton();

	UFUNCTION()
	void HandleClick(TArray<int32> IsHoldings);
};

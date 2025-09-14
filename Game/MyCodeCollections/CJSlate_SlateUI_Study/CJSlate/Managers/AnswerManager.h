// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AnswerManager.generated.h"

class ADoorActor;

UENUM(BlueprintType)
enum class EAnswerType : uint8
{
	AdditiveAnswer   UMETA(DisplayName = "AdditiveAnswer"), // 축적되는 답인 경우, ex) "1" -> "12" -> "123"
	BitAnswer        UMETA(DisplayName = "BitAnswer"),	// Bit를 바꾸는 답인 경우, ex) "0000" -> "1000" -> "1100"
	ColorAnswer        UMETA(DisplayName = "ColorAnswer"),	// 색을 바꾸는 답인 경우, ex) "" -> "R" -> "RG" -> "RGB"
	DirectionalLockAnswer        UMETA(DisplayName = "DirectionalLockAnswer"),
	ButtonLockAnswer        UMETA(DisplayName = "ButtonLockAnswer"),
	CombinationLockAnswer        UMETA(DisplayName = "CombinationLockAnswer"),
	Max
};

// ** 기본적으로 서버에서 실행됨 */
UCLASS()
class CJSLATE_API AAnswerManager : public AActor
{
	GENERATED_BODY()

public:
	AAnswerManager();

protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	virtual void Tick(float DeltaTime) override;

	// 초기화할 대상들. 아니면 모두 어떤 특정 값을 추가할때도 쓰임.
	// 즉 지금 관리하고 있는 대상들
	UPROPERTY(EditAnywhere, Replicated)
	TArray<TSoftObjectPtr<AActor>> Actors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Answer")
	FString CorrectAnswer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Answer")
	FString CurrentAnswer = "";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Answer")
	EAnswerType AnswerType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	FString ManagerName;

	void AddAnswer(int32 index);
	void BitmaskAnswer(int32 index, bool value);
	void BitmaskValueAnswer(int32 index, int32 value);
	void IsCorrectAnswer();
	
	UFUNCTION(BlueprintCallable)
	void ClearAnswers();

	// 자물쇠에서 열으려고 하는 문
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category= "Answer")
	TArray<TObjectPtr<ADoorActor>> Doors;

	// 문제가 해결됐는지 확인하는 변수
	bool IsCleared = false;

	UFUNCTION(NetMulticast, Reliable)
	void PopUpAnswerUI(const FString& str);
};
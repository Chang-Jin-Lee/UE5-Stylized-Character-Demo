// Fill out your copyright notice in the Description page of Project Settings.


#include "AnswerManager.h"

#include "CJSlate/Beacon/BeaconActor.h"
#include "CJSlate/Door/DoorActor.h"
#include "CJSlate/HUD/CJSlateHUD.h"
#include "CJSlate/Stone/StoneActor.h"
#include "CJSlate/UI/InGamePanelWidget.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


AAnswerManager::AAnswerManager()
{
	PrimaryActorTick.bCanEverTick = true;
	
	bReplicates = true;
	SetReplicateMovement(true);
	
	AnswerType = EAnswerType::Max;
}

void AAnswerManager::BeginPlay()
{
	Super::BeginPlay();
}

void AAnswerManager::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAnswerManager, Actors);
	DOREPLIFETIME(AAnswerManager, Doors);
	DOREPLIFETIME(AAnswerManager, CorrectAnswer);
	DOREPLIFETIME(AAnswerManager, CurrentAnswer);
	DOREPLIFETIME(AAnswerManager, AnswerType);
}

void AAnswerManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAnswerManager::AddAnswer(int32 index)
{
	CurrentAnswer += FString::FromInt(index);
}

void AAnswerManager::BitmaskAnswer(int32 index, bool value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("LockActor %s"), *CurrentAnswer));
	if (CurrentAnswer.Len() > index)
	{
		value == true ? CurrentAnswer[index] = '1' : CurrentAnswer[index] = '0';
	}
}

void AAnswerManager::BitmaskValueAnswer(int32 index, int32 value)
{
	if (CurrentAnswer.Len() > index)
	{
		CurrentAnswer[index] = static_cast<TCHAR>(value % 10 + '0');
	}
}

// 서버에서 실행됨
void AAnswerManager::IsCorrectAnswer()
{
	// 문제를 해결했다면 더 이상 정답인지 확인하지 않음
	if (IsCleared == true) return;
	
	switch (AnswerType)
	{
	case EAnswerType::AdditiveAnswer:
		if (CorrectAnswer == CurrentAnswer)
		{
			// 정답일 경우
			// 문을 열기
			if (Doors.IsEmpty() == false)
			{
				for (auto Door : Doors)
				{
					Door->MoveData.EventTimeline->PlayFromStart();
				}
				IsCleared = true;
				PopUpAnswerUI("Correct Answer! Open Door...");
			}
		}
		else if (CurrentAnswer.Len() == CorrectAnswer.Len())
		{
			if (CurrentAnswer != CorrectAnswer)
			{
				PopUpAnswerUI("Wrong Answer!...");
				ClearAnswers();
			}
		}
		break;
	case EAnswerType::BitAnswer:
		if (CorrectAnswer == CurrentAnswer)
		{
			// 정답일 경우
			if (Doors.IsEmpty() == false)
			{
				for (auto Door : Doors)
				{
					Door->MoveData.EventTimeline->PlayFromStart();
				}
				IsCleared = true;
				PopUpAnswerUI("Correct Answer! Open Door...");
			}
		}
		break;
	case EAnswerType::ColorAnswer:
		if (CorrectAnswer == CurrentAnswer)
		{
			// 정답일 경우
			if (Doors.IsEmpty() == false)
			{
				for (auto Door : Doors)
				{
					if (Door->IsOpened == false)
						Door->OpenDoor();
				}
				IsCleared = true;
				PopUpAnswerUI("Correct Answer! Open Door...");
			}
		}
		break;
	case EAnswerType::DirectionalLockAnswer:	// 자물쇠 관련, (방향 자물쇠, 버튼 자물쇠해당)
		if (CorrectAnswer == CurrentAnswer)
		{
			// 정답일 경우
			// 문을 열기
			if (Doors.IsEmpty() == false)
			{
				for (auto Door : Doors)
				{
					Door->MoveData.EventTimeline->PlayFromStart();
				}
				IsCleared = true;
				PopUpAnswerUI("Unlock! Open Something...");
			}
		}
		else if (CurrentAnswer.Len() == CorrectAnswer.Len())
		{
			if (CurrentAnswer != CorrectAnswer)
			{
				PopUpAnswerUI("Wrong Answer!...");
				ClearAnswers();
			}
		}
		break;
	case EAnswerType::ButtonLockAnswer:	// 자물쇠 관련, (조합 자물쇠 해당)
		if (CorrectAnswer == CurrentAnswer)
		{
			// 정답일 경우
			// 문을 열기
			if (Doors.IsEmpty() == false)
			{
				for (auto Door : Doors)
				{
					Door->MoveData.EventTimeline->PlayFromStart();
				}
				IsCleared = true;
				PopUpAnswerUI("Unlock! Open Something...");
			}
		}
		break;
	case EAnswerType::CombinationLockAnswer:	// 자물쇠 관련, (조합 자물쇠 해당)
		if (CorrectAnswer == CurrentAnswer)
		{
			// 정답일 경우
			// 문을 열기
			if (Doors.IsEmpty() == false)
			{
				for (auto Door : Doors)
				{
					Door->MoveData.EventTimeline->PlayFromStart();
				}
				IsCleared = true;
				PopUpAnswerUI("Unlock! Open Something...");
			}
		}
		break;
	case EAnswerType::Max:
		break;
	}
	
}

void AAnswerManager::ClearAnswers()
{
	CurrentAnswer = "";
	for (auto Actor : Actors)
	{
		if ( ABeaconActor* Beacon = Cast<ABeaconActor>(Actor.Get()))
		{
			Beacon->SetActorLocation(Beacon->StartingLocation);
			Beacon->CollisionBox->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndProbe);
		}
		else if ( AStoneActor* Stone = Cast<AStoneActor>(Actor.Get()))
		{
			if (CurrentAnswer.Len() > Stone->StoneID)
				CurrentAnswer[Stone->StoneID] = '0';
			Stone->MeshComp->SetRelativeLocation(Stone->MoveData.InitialLocation);
		}
	}
}

void AAnswerManager::PopUpAnswerUI_Implementation(const FString& str)
{
	ACJSlateHUD* HUD = Cast<ACJSlateHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (HUD && HUD->m_wHUDPanelWidget)
	{
		HUD->m_wHUDPanelWidget->PopUp(FText::FromString(str));
	}
}

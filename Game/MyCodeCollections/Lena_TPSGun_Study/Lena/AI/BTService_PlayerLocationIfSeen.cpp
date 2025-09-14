// Fill out your copyright notice in the Description page of Project Settings.


#include "Lena/AI/BTService_PlayerLocationIfSeen.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Lena/Lena.h"
#include "Lena/Characters/AI_Character.h"
#include "Lena/Characters/Base_Character.h"
#include "Lena/UI/Lena_InGameAIHealthBarWidget.h"


UBTService_PlayerLocationIfSeen::UBTService_PlayerLocationIfSeen()
{
	NodeName = TEXT("Update Player Location If Seen");
}

void UBTService_PlayerLocationIfSeen::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	if (AAIController* AIController = OwnerComp.GetAIOwner())
	{
		if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
		{
			if (APawn* AIPawn = AIController->GetPawn())
			{
				if (AAI_Character* ShooterCharacter = Cast<AAI_Character>(AIPawn))
				{
					bool bHasLineOfSight = AIController->LineOfSightTo(PlayerPawn);
					// 시야각 추가 검사 (필요 시)
					FVector DirectionToPlayer = (PlayerPawn->GetActorLocation() - AIPawn->GetActorLocation()).GetSafeNormal();
					FVector ForwardVector = AIPawn->GetActorForwardVector();
					float AngleToPlayer = FMath::Acos(FVector::DotProduct(ForwardVector, DirectionToPlayer)) * (180.0f / PI);

					if (bHasLineOfSight && AngleToPlayer <= AICHARACTER_UI_VISIBLE_FOV / 2)
					{
						ShooterCharacter->ToggleIsArmed();
						ShooterCharacter->DrawWeaponEvent();
						ShooterCharacter->AIAimTarget();
						AIController->GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), PlayerPawn);
						
					}
					else
					{
						ShooterCharacter->ToggleIsArmed();
						ShooterCharacter->AILooseAimTarget();
						ShooterCharacter->PutDownWeaponEvent();
						AIController->GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
					}
				}
			}
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Lena/AI/BTTask_Shoot.h"
#include "AIController.h"
#include "Lena/Characters/AI_Character.h"
#include "Lena/Characters/Base_Character.h"

UBTTask_Shoot::UBTTask_Shoot()
{
	NodeName = TEXT("Shoot");
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	AAIController *AIController =  OwnerComp.GetAIOwner();
	if(AIController == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	if(APawn* AIPawn = AIController->GetPawn())
	{
		if(AAI_Character* ShooterCharacter = Cast<AAI_Character>(AIPawn))
		{
			if(!ShooterCharacter)
			{
				return EBTNodeResult::Failed;
			}
			ShooterCharacter->ToggleIsArmed();
			if(TObjectPtr<ABase_Weapon> Weapon = ShooterCharacter->CurrentWeapon)
			{
				if(ShooterCharacter->GetAmmoIndex(Weapon->AmmoType) > 0)
				{
					ShooterCharacter->ReloadAIAction();
				}
			}
			// UE_LOG(LogTemp, Warning, TEXT("AIShooterCharacter->GetAmmo() : %d"), ShooterCharacter->GetAmmoIndex(Weapon->AmmoType));
			ShooterCharacter->PerformAction();
			// if(ShooterCharacter == nullptr)
			// {
			// 	return EBTNodeResult::Failed;
			// }
			//
			// ShooterCharacter->ToggleIsArmed();
			//
			// UE_LOG(LogTemp, Warning, TEXT("Health Left : %d"), ShooterCharacter->GetAmmo());
			//
			// if(!ShooterCharacter->GetAmmo())
			// {
			// 	ShooterCharacter->ReloadAIAction();
			// }
			//
			// UE_LOG(LogTemp, Warning, TEXT("Health Left : %d"), ShooterCharacter->GetAmmo());
			//
			// ShooterCharacter->Shoot();
		}
	}
	return EBTNodeResult::Succeeded;
}
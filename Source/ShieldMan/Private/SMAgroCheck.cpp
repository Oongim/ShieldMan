// Fill out your copyright notice in the Description page of Project Settings.


#include "SMAgroCheck.h"
#include "SMSlimeAI.h"
#include "SMSlime.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "BehaviorTree/BehaviorTree.h"
#include "DrawDebugHelpers.h"


void USMAgroCheck::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (ThisTree == nullptr || ThisController == nullptr || ThisAICharacter == nullptr)
	{
		ThisTree = OwnerComp.GetCurrentTree();
		ThisController = Cast<ASMSlimeAI>(OwnerComp.GetAIOwner());
		ThisAICharacter = Cast<ASMSlime>(ThisController->GetPawn());

		if (ThisTree == nullptr || ThisController == nullptr || ThisAICharacter == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Warning AgroService ~~"));
			return;
		}
	}

	FCollisionQueryParams SphereSweepParams(FName(TEXT("AgroCheckSweep")), true, ThisAICharacter);
	FCollisionObjectQueryParams ObjectQuery(ECC_GameTraceChannel1);		//
	FHitResult HitOut(ForceInit);

	DrawDebugSphere(ThisAICharacter->GetWorld(), ThisAICharacter->GetActorLocation(), 1500, 12, FColor::Red, false, 4.0f);

	bool bResult = ThisAICharacter->GetWorld()->SweepSingleByObjectType(HitOut,
		ThisAICharacter->GetActorLocation(),
		ThisAICharacter->GetActorLocation() + FVector(0.f, 0.f, 0.f),
		FQuat(),
		ObjectQuery,
		FCollisionShape::MakeSphere(1500),
		SphereSweepParams);

	if (bResult)
	{
		ThisController->GetBlackBoard()->SetValueAsObject(TEXT("Character"), HitOut.GetActor());
		ThisController->GetBlackBoard()->SetValueAsVector(TEXT("HomeLocation"), ThisAICharacter->GetActorLocation());
		ThisController->GetBlackBoard()->SetValueAsVector(TEXT("CharacterLocation"), HitOut.GetActor()->GetActorLocation());
	}
	else
	{
		ThisController->GetBlackBoard()->SetValueAsObject(TEXT("Character"), nullptr);
	}


}
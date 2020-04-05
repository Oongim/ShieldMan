// Fill out your copyright notice in the Description page of Project Settings.


#include "SMSlimeAI.h"
#include "SMSlime.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

ASMSlimeAI::ASMSlimeAI()
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
	check(BlackboardComp);
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComp"));
	check(BehaviorTreeComp);
}

void ASMSlimeAI::PossessedBy(APawn* InPawn)
{
	Super::Possess(InPawn);

	ASMSlime* possessdPawn = Cast<ASMSlime>(InPawn);

	if (InPawn != nullptr)
	{
		BlackboardComp->InitializeBlackboard(*(possessdPawn->BehaviorTree->BlackboardAsset));

		BehaviorTreeComp->StartTree(*(possessdPawn->BehaviorTree));
	}

}

UBlackboardComponent* ASMSlimeAI::GetBlackBoard()
{
	return BlackboardComp;
}


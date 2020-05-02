// Fill out your copyright notice in the Description page of Project Settings.


#include "SMSlimeAI.h"
#include "SMSlime.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"



ASMSlimeAI::ASMSlimeAI()
{
	RepeatInterval = 3.0f;
	UE_LOG(LogTemp, Warning, TEXT("ASMSlimeAI"));

	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
	check(BlackboardComp);
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComp"));
	check(BehaviorTreeComp);
}

void ASMSlimeAI::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &ASMSlimeAI::OnRepeatTimer, RepeatInterval, true);
	ULog::Invalid("OnPossess", "", LO_Viewport);
}

void ASMSlimeAI::OnUnPossess()
{
	Super::OnUnPossess();
	GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHandle);
	ULog::Invalid("OnUnPossess", "", LO_Viewport);
}

void ASMSlimeAI::OnRepeatTimer()
{
	
	auto CurrentPawn = GetPawn();
	if(CurrentPawn==nullptr)	ULog::Invalid("CurrentPawn", "", LO_Viewport);

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (nullptr == NavSystem) return;

	ULog::Invalid("OnRepeatTimer", "", LO_Viewport);
	FNavLocation NextLocation;
	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.f, NextLocation))
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, NextLocation.Location);

		ULog::Vector(NextLocation.Location, "MyActor location: ", "", LO_Viewport);
	}

}
void ASMSlimeAI::PossessedBy(APawn* InPawn)
{
	Super::Possess(InPawn);

	/*ASMSlime* possessdPawn = Cast<ASMSlime>(InPawn);

	if (InPawn != nullptr)
	{
		BlackboardComp->InitializeBlackboard(*(possessdPawn->BehaviorTree->BlackboardAsset));

		BehaviorTreeComp->StartTree(*(possessdPawn->BehaviorTree));
	}*/

}

UBlackboardComponent* ASMSlimeAI::GetBlackBoard()
{
	return BlackboardComp;
}


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SMSlimeAI.generated.h"

/**
 * 
 */
UCLASS()
class SHIELDMAN_API ASMSlimeAI : public AAIController
{
	GENERATED_BODY()
	

public:
	ASMSlimeAI();
	virtual void PossessedBy(APawn* InPawn);

	UBlackboardComponent* GetBlackBoard();

protected:
	UPROPERTY(BlueprintReadWrite, Category = Behavior)
		class UBlackboardComponent* BlackboardComp;

	UPROPERTY(BlueprintReadWrite, Category = Behavior)
		class UBehaviorTreeComponent* BehaviorTreeComp;

};

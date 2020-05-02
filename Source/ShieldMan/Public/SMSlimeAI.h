// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ShieldMan.h"
#include "AIController.h"
#include "SMSlimeAI.generated.h"

/**
 * 
 */
UCLASS()
class SHIELDMAN_API ASMSlimeAI : public AAIController
{
	GENERATED_BODY()
	
private:
	FTimerHandle RepeatTimerHandle;
	float RepeatInterval;

private:
	void OnRepeatTimer();

public:
	ASMSlimeAI();
	virtual void PossessedBy(APawn* InPawn);
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	UBlackboardComponent* GetBlackBoard();

protected:
	UPROPERTY(BlueprintReadWrite, Category = Behavior)
		class UBlackboardComponent* BlackboardComp;

	UPROPERTY(BlueprintReadWrite, Category = Behavior)
		class UBehaviorTreeComponent* BehaviorTreeComp;

};

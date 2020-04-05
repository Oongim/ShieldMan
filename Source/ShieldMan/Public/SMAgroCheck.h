// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SMAgroCheck.generated.h"

/**
 * 
 */
UCLASS()
class SHIELDMAN_API USMAgroCheck : public UBTService
{
	GENERATED_BODY()
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UBehaviorTree* ThisTree;
	class ASMSlimeAI* ThisController;
	class ASMSlime* ThisAICharacter;
};

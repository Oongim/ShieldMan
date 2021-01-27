// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ShieldMan.h"
#include "SM_Ghost_AttackObject.h"
#include "SM_BossGhost_AttackObject.generated.h"

/**
 * 
 */
UCLASS()
class SHIELDMAN_API ASM_BossGhost_AttackObject : public ASM_Ghost_AttackObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Class")
		TSubclassOf<class AMetaBall_Boss> BossClass;
public:
	virtual void SetTarget(AActor* actor) override;

};

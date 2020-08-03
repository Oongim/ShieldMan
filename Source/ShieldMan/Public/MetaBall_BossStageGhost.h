// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MetaBall_Ghost.h"
#include "MetaBall_BossStageGhost.generated.h"

/**
 * 
 */
UCLASS()
class SHIELDMAN_API AMetaBall_BossStageGhost : public AMetaBall_Ghost
{
	GENERATED_BODY()
private:
	bool bAttack = false;
protected:
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnRepeatTimer() override;

	virtual void MoveStart() override;
};

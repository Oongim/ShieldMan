// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MetaBall_Slime.h"
#include "MetaBall_BossStageSlime.generated.h"

/**
 * 
 */
UCLASS()
class SHIELDMAN_API AMetaBall_BossStageSlime : public AMetaBall_Slime
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

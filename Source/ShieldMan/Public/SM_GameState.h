// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ShieldMan.h"
#include "GameFramework/GameStateBase.h"
#include "SM_GameState.generated.h"

/**
 * 
 */
UCLASS()
class SHIELDMAN_API ASM_GameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	UPROPERTY(Replicated)
		FRotator ControllerRot;

	UPROPERTY(Replicated)
		FVector RightHandPos;

	UPROPERTY(Replicated)
		FVector LeftHandPos;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
};

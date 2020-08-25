// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ShieldMan.h"
#include "GameFramework/PlayerState.h"
#include "SM_PlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SHIELDMAN_API ASM_PlayerState : public APlayerState
{
	GENERATED_BODY()
private:
	class ASM_GameState* GameState;

public:
	UPROPERTY(Replicated)
	FRotator ControllerRot;

	UPROPERTY(Replicated)
		FVector RightHandPos;
	UPROPERTY(Replicated)
		FVector LeftHandPos;

	class ASM_PlayerState* Main_PlayerState;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	void InitPlayerData(ASM_GameState* game_state,FString name);

	FRotator GetControllerRot();
};

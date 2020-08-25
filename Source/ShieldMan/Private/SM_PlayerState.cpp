// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "SM_GameState.h"

void ASM_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASM_PlayerState, ControllerRot);
	DOREPLIFETIME(ASM_PlayerState, RightHandPos);
	DOREPLIFETIME(ASM_PlayerState, LeftHandPos);
}

void ASM_PlayerState::InitPlayerData(ASM_GameState* game_state, FString name)
{
	GEngine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("InitPlayerData")));
	GameState = game_state;

	SetPlayerName(name);

	if (FString{ "Body" } != name) {
		for (auto PS : GameState->PlayerArray) {
			if (PS->GetPlayerName() == FString{ "Body" }) {
				Main_PlayerState = Cast<ASM_PlayerState>(PS);
				GEngine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("BodySetting")));
			}
		}
	}
}

FRotator ASM_PlayerState::GetControllerRot()
{
	if (nullptr != Main_PlayerState) {
		ControllerRot = Main_PlayerState->ControllerRot;
	}

	return ControllerRot;
}

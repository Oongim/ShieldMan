// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_GameState.h"
#include "Net/UnrealNetwork.h"

void ASM_GameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASM_GameState, ControllerRot);
	DOREPLIFETIME(ASM_GameState, RightHandPos);
	DOREPLIFETIME(ASM_GameState, LeftHandPos);
}
     
// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_GameState.h"
#include "Net/UnrealNetwork.h"

void ASM_GameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASM_GameState, ControllerRot);
	//DOREPLIFETIME(ASM_GameState, RightHandPos);
	//DOREPLIFETIME(ASM_GameState, LeftHandPos);

	DOREPLIFETIME(ASM_GameState, Stage1);
	DOREPLIFETIME(ASM_GameState, Stage2);
	DOREPLIFETIME(ASM_GameState, Stage3);
	DOREPLIFETIME(ASM_GameState, EntireTime);
}
     
void ASM_GameState::SetTimeStage1(float time)
{
	Stage1 = time;
}
void ASM_GameState::SetTimeStage2(float time)
{
	Stage2 = time;
}
void ASM_GameState::SetTimeStage3(float time)
{
	Stage3 = time;
}

float ASM_GameState::GetTimeStage1()
{
	return Stage1;
}
float ASM_GameState::GetTimeStage2()
{
	return Stage2;
}
float ASM_GameState::GetTimeStage3()
{
	return Stage3;
}

void ASM_GameState::AddEntireTime(float time)
{
	EntireTime += time;
}

float ASM_GameState::GetEntireTime()
{
	return EntireTime;
}

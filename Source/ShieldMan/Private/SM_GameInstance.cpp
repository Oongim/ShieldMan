// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_GameInstance.h"

USM_GameInstance::USM_GameInstance()
{
	Stage1 = 0;
	Stage2 = 0;
	Stage3 = 0;
	PlayerNumber = 0;
}


void USM_GameInstance::SetPlayerNumber(int num)
{
	PlayerNumber = num;
}
int USM_GameInstance::GetPlayerNumber()
{
	return PlayerNumber;
}

void USM_GameInstance::SetTimeStage1(float time)
{
	Stage1 = time;
}
void USM_GameInstance::SetTimeStage2(float time)
{
	Stage2 = time;
}
void USM_GameInstance::SetTimeStage3(float time)
{
	Stage3 = time;
}

float USM_GameInstance::GetTimeStage1()
{
	return Stage1;
}
float USM_GameInstance::GetTimeStage2()
{
	return Stage2;
}
float USM_GameInstance::GetTimeStage3()
{
	return Stage3;
}
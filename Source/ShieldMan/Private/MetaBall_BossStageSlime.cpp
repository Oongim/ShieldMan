// Fill out your copyright notice in the Description page of Project Settings.


#include "MetaBall_BossStageSlime.h"

void AMetaBall_BossStageSlime::BeginPlay()
{
	Super::BeginPlay();

	MoveStart();
	setSpeedPower(0.8f);
	ShakeClamp = 200.f;
	RunAwayPower = 30.f;
}


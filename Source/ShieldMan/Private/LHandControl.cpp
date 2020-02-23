// Fill out your copyright notice in the Description page of Project Settings.


#include "LHandControl.h"

LHandControl::LHandControl()
{
	Initialize();
}

LHandControl::~LHandControl()
{
}

void LHandControl::Initialize()
{
	SetModeName(LHandControlMode);
}
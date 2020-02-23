// Fill out your copyright notice in the Description page of Project Settings.


#include "RHandControl.h"

RHandControl::RHandControl()
{
	Initialize();
}

RHandControl::~RHandControl()
{
}

void RHandControl::Initialize()
{
	SetModeName(RHandControlMode);
}
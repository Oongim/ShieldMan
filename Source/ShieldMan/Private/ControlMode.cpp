// Fill out your copyright notice in the Description page of Project Settings.


#include "ControlMode.h"

ControlMode::ControlMode()
{
}

ControlMode::~ControlMode()
{
}

bool ControlMode::isControlMode(FName name)
{
	return modeName == name;
}

void ControlMode::SetModeName(FName name)
{
	modeName = name;
}

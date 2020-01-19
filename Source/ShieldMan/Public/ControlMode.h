// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ShieldMan.h"

/**
 * 
 */
class SHIELDMAN_API ControlMode
{
private:
	FName modeName;

public:
	ControlMode();
	virtual ~ControlMode();

public:
	virtual void Initialize() = 0;
	bool isControlMode(FName name);
	void SetModeName(FName name);
};

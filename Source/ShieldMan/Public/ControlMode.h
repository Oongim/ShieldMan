// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ShieldMan.h"

/**
 * 
 */
class SHIELDMAN_API ControlMode
{
private:
	EControlMode modeName;

public:
	ControlMode();
	virtual ~ControlMode();

public:
	virtual void Initialize() = 0;
	bool isControlMode(EControlMode name);
	void SetModeName(EControlMode name);
};
